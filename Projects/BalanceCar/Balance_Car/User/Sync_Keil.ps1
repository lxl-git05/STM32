# Run after CubeMX Generate Code, with the Keil project closed.
$ErrorActionPreference = 'Stop'
$projectPath = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../MDK-ARM/Balance_Car.uvprojx'))
$projectDirectory = Split-Path -Parent $projectPath
$content = [IO.File]::ReadAllText($projectPath)
[xml]$projectXml = $content
if (@($projectXml.Project.Targets.Target).Count -ne 1) {
    throw 'This script expects one Keil target. Configure additional targets explicitly.'
}

# Every module subdirectory is included, so new device/algorithm folders work too.
$directories = @(Get-ChildItem -LiteralPath $PSScriptRoot -Directory -Recurse | Sort-Object FullName)
$includePaths = @($directories | ForEach-Object {
    '../User/' + $_.FullName.Substring($PSScriptRoot.Length + 1).Replace('\', '/')
})
$includePattern = '(<Cads>[\s\S]*?<VariousControls>[\s\S]*?<IncludePath>)([^<]*)(</IncludePath>)'
$includeRegex = [regex]::new($includePattern)
if (-not $includeRegex.IsMatch($content)) { throw 'Target C include path was not found.' }
$content = $includeRegex.Replace($content, [System.Text.RegularExpressions.MatchEvaluator]{
    param($match)
    $paths = @($match.Groups[2].Value.Split(';') | Where-Object { $_ })
    foreach ($path in $includePaths) {
        if ($paths -notcontains $path) { $paths += $path }
    }
    $match.Groups[1].Value + ($paths -join ';') + $match.Groups[3].Value
}, 1)

$layers = @('Initial', 'AllHeader', 'ISR', 'Task', 'Msg', 'Hardware', 'Software', 'Function')
foreach ($layer in $layers) {
    $groupName = 'User/' + $layer
    $group = "        <Group>`r`n          <GroupName>$groupName</GroupName>`r`n          <Files>`r`n"
    foreach ($file in @(Get-ChildItem -LiteralPath (Join-Path $PSScriptRoot $layer) -File -Recurse | Where-Object { $_.Extension -in '.c', '.h' } | Sort-Object FullName)) {
        $fileType = if ($file.Extension -eq '.c') { 1 } else { 5 }
        $filePath = '../User/' + $file.FullName.Substring($PSScriptRoot.Length + 1).Replace('\', '/')
        $escapedName = [Security.SecurityElement]::Escape($file.Name)
        $escapedPath = [Security.SecurityElement]::Escape($filePath)
        $group += "            <File>`r`n              <FileName>$escapedName</FileName>`r`n              <FileType>$fileType</FileType>`r`n              <FilePath>$escapedPath</FilePath>`r`n            </File>`r`n"
    }
    $group += "          </Files>`r`n        </Group>`r`n"
    $pattern = '(?m)^        <Group>\r?\n\s*<GroupName>' + [regex]::Escape($groupName) + '</GroupName>[\s\S]*?</Group>\r?\n'
    $groupRegex = [regex]::new($pattern)
    if ($groupRegex.IsMatch($content)) {
        $content = $groupRegex.Replace($content, [System.Text.RegularExpressions.MatchEvaluator]{ param($match) $group })
    } else {
        $content = $content.Replace('      </Groups>', $group + '      </Groups>')
    }
}
[xml]$validatedXml = $content
[IO.File]::WriteAllText($projectPath, $content, [Text.UTF8Encoding]::new($false))
Write-Output 'Keil User include paths and groups synchronized.'
