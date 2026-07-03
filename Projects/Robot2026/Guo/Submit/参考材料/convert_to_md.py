# -*- coding: utf-8 -*-
"""
Convert DOCX XML to Markdown while preserving styles, headings, lists, and images
"""
import os
import re
from xml.etree import ElementTree as ET

# Define namespaces
NAMESPACES = {
    'w': 'http://schemas.openxmlformats.org/wordprocessingml/2006/main',
    'wp': 'http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing',
    'a': 'http://schemas.openxmlformats.org/drawingml/2006/main',
    'pic': 'http://schemas.openxmlformats.org/drawingml/2006/picture',
    'r': 'http://schemas.openxmlformats.org/officeDocument/2006/relationships',
}

# Register namespaces
for prefix, uri in NAMESPACES.items():
    ET.register_namespace(prefix if prefix != 'w' else '', uri)

# Also register other namespaces found in the document
ET.register_namespace('wpc', 'http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas')
ET.register_namespace('mc', 'http://schemas.openxmlformats.org/markup-compatibility/2006')
ET.register_namespace('o', 'urn:schemas-microsoft-com:office:office')
ET.register_namespace('m', 'http://schemas.openxmlformats.org/officeDocument/2006/math')
ET.register_namespace('v', 'urn:schemas-microsoft-com:vml')
ET.register_namespace('wp14', 'http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing')
ET.register_namespace('w14', 'http://schemas.microsoft.com/office/word/2010/wordml')
ET.register_namespace('w10', 'urn:schemas-microsoft-com:office:word')
ET.register_namespace('w15', 'http://schemas.microsoft.com/office/word/2012/wordml')
ET.register_namespace('wpg', 'http://schemas.microsoft.com/office/word/2010/wordprocessingGroup')
ET.register_namespace('wpi', 'http://schemas.microsoft.com/office/word/2010/wordprocessingInk')
ET.register_namespace('wne', 'http://schemas.microsoft.com/office/word/2006/wordml')
ET.register_namespace('wps', 'http://schemas.microsoft.com/office/word/2010/wordprocessingShape')
ET.register_namespace('wpsCustomData', 'http://wps.cn/officeDocument/2013/wpsCustomData')
ET.register_namespace('a14', 'http://schemas.microsoft.com/office/drawing/2010/main')


def get_text_from_r(r_elem):
    """Extract text from a run element"""
    text_parts = []
    for t in r_elem.findall('.//w:t', NAMESPACES):
        if t.text:
            text_parts.append(t.text)
    return ''.join(text_parts)


def get_bold_from_r(r_elem):
    """Check if run has bold formatting"""
    rPr = r_elem.find('w:rPr', NAMESPACES)
    if rPr is not None:
        b = rPr.find('w:b', NAMESPACES)
        if b is not None:
            b_val = b.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')
            if b_val is None or b_val == '1':
                return True
    return False


def get_italic_from_r(r_elem):
    """Check if run has italic formatting"""
    rPr = r_elem.find('w:rPr', NAMESPACES)
    if rPr is not None:
        i = rPr.find('w:i', NAMESPACES)
        if i is not None:
            i_val = i.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')
            if i_val is None or i_val == '1':
                return True
    return False


def get_underline_from_r(r_elem):
    """Check if run has underline formatting"""
    rPr = r_elem.find('w:rPr', NAMESPACES)
    if rPr is not None:
        u = rPr.find('w:u', NAMESPACES)
        if u is not None:
            u_val = u.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')
            if u_val and u_val != 'none':
                return True
    return False


def get_font_size(r_elem):
    """Get font size from run"""
    rPr = r_elem.find('w:rPr', NAMESPACES)
    if rPr is not None:
        sz = rPr.find('w:sz', NAMESPACES)
        if sz is not None:
            return sz.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')
    return None


def get_alignment(p_elem):
    """Get paragraph alignment"""
    pPr = p_elem.find('w:pPr', NAMESPACES)
    if pPr is not None:
        jc = pPr.find('w:jc', NAMESPACES)
        if jc is not None:
            return jc.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')
    return None


def get_indentation(p_elem):
    """Get paragraph indentation"""
    pPr = p_elem.find('w:pPr', NAMESPACES)
    if pPr is not None:
        ind = pPr.find('w:ind', NAMESPACES)
        if ind is not None:
            left = ind.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}left')
            firstLine = ind.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}firstLine')
            return {
                'left': int(left) if left else 0,
                'firstLine': int(firstLine) if firstLine else 0
            }
    return {'left': 0, 'firstLine': 0}


def get_heading_info(p_elem):
    """Get heading level and text based on content analysis"""
    texts = []
    for r in p_elem.findall('.//w:r', NAMESPACES):
        texts.append(get_text_from_r(r))

    full_text = ''.join(texts).strip()
    if not full_text:
        return None, ''

    # Check for chapter-like headings
    chapter_match = re.match(r'^第[一二三四五六七八九十]+章\s*(.*)', full_text)
    if chapter_match:
        inner_text = chapter_match.group(1).strip()
        return 1, inner_text if inner_text else full_text

    # Check for numbered sections like "3.1", "3.1.1"
    numbered_match = re.match(r'^(\d+\.\d+(?:\.\d+)?)\s*(.*)', full_text)
    if numbered_match:
        num = numbered_match.group(1)
        inner_text = numbered_match.group(2).strip()
        if '.' in num:
            parts = num.split('.')
            if len(parts) == 2:
                return 2, inner_text if inner_text else full_text
            elif len(parts) >= 3:
                return 3, inner_text if inner_text else full_text

    # Check for section titles like "一、项目题目"
    section_match = re.match(r'^[一二三四五六七八九十]、\s*(.*)', full_text)
    if section_match:
        inner_text = section_match.group(1).strip()
        return 2, full_text  # Keep the full text for section titles

    # Check font size for heading level
    for r in p_elem.findall('.//w:r', NAMESPACES):
        sz = get_font_size(r)
        b = get_bold_from_r(r)
        if sz and int(sz) >= 44 and b:  # 22pt or larger, bold
            return 1, full_text
        elif sz and int(sz) >= 36 and b:  # 18pt, bold
            return 2, full_text
        elif sz and int(sz) >= 28 and b:  # 14pt, bold
            return 3, full_text

    return None, full_text


def get_list_info(p_elem):
    """Get list numbering info"""
    pPr = p_elem.find('w:pPr', NAMESPACES)
    if pPr is not None:
        numPr = pPr.find('w:numPr', NAMESPACES)
        if numPr is not None:
            ilvl = numPr.find('w:ilvl', NAMESPACES)
            numId = numPr.find('w:numId', NAMESPACES)
            return {
                'level': int(ilvl.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')) if ilvl is not None else 0,
                'numId': int(numId.get('{http://schemas.openxmlformats.org/wordprocessingml/2006/main}val')) if numId is not None else 0
            }
    return None


def get_image_info(r_elem, rels):
    """Extract image info from a drawing element"""
    drawing = r_elem.find('.//w:drawing', NAMESPACES)
    if drawing is None:
        return None

    # Try inline first
    inline = drawing.find('.//wp:inline', NAMESPACES)
    if inline is not None:
        extent = inline.find('wp:extent', NAMESPACES)
        docPr = inline.find('wp:docPr', NAMESPACES)
        blip = inline.find('.//a:blip', NAMESPACES)

        if blip is not None:
            embed = blip.get('{http://schemas.openxmlformats.org/officeDocument/2006/relationships}embed')
            if embed and embed in rels:
                return {
                    'path': rels[embed],
                    'descr': docPr.get('descr', '') if docPr is not None else '',
                    'name': docPr.get('name', '') if docPr is not None else ''
                }

    return None


def process_run(r_elem):
    """Process a single run and return formatted text"""
    text = get_text_from_r(r_elem)
    if not text:
        return ''

    is_bold = get_bold_from_r(r_elem)
    is_italic = get_italic_from_r(r_elem)
    is_underline = get_underline_from_r(r_elem)

    result = text
    if is_bold and not (result.startswith('**') and result.endswith('**')):
        result = f'**{result}**'
    if is_italic and not (result.startswith('*') and result.endswith('*')):
        result = f'*{result}*'
    if is_underline and not result.startswith('<u>'):
        result = f'<u>{result}</u>'

    return result


def process_paragraph(p_elem, numbering, rels):
    """Process a paragraph and return markdown lines"""
    # Collect all text runs and images
    text_runs = []
    images = []

    for r in p_elem.findall('.//w:r', NAMESPACES):
        img_info = get_image_info(r, rels)
        if img_info:
            images.append(img_info)
        else:
            text_runs.append(process_run(r))

    # Get paragraph properties
    pPr = p_elem.find('w:pPr', NAMESPACES)
    alignment = get_alignment(p_elem) if pPr is not None else None
    indent = get_indentation(p_elem) if pPr is not None else {'left': 0, 'firstLine': 0}
    list_info = get_list_info(p_elem) if pPr is not None else None

    # Combine text
    full_text = ''.join(text_runs).strip()

    # Check for heading
    heading_level, heading_text = get_heading_info(p_elem)

    lines = []

    if heading_level:
        prefix = '#' * heading_level + ' '
        lines.append(f'{prefix}{heading_text}')
    elif list_info:
        # List item
        level = list_info['level']
        indent_str = '  ' * level
        lines.append(f'{indent_str}- {full_text}')
    else:
        # Regular paragraph
        if indent['left'] > 500 or indent['firstLine'] > 0:
            lines.append(f'    {full_text}')
        elif full_text:
            lines.append(full_text)
        else:
            lines.append('')

    # Add images after text
    for img in images:
        img_path = os.path.basename(img['path'])
        if img['descr']:
            lines.append(f'![{img["descr"]}](media/{img_path})')
        else:
            lines.append(f'![](media/{img_path})')

    return lines


def convert_docx_xml_to_md(xml_path, output_path, rels_path):
    """Main conversion function"""
    # Load relationships
    rels_tree = ET.parse(rels_path)
    rels_root = rels_tree.getroot()
    rels = {}
    for rel in rels_root.findall('.//{http://schemas.openxmlformats.org/package/2006/relationships}Relationship'):
        rel_id = rel.get('Id')
        rel_target = rel.get('Target')
        rels[rel_id] = rel_target

    # Parse document
    tree = ET.parse(xml_path)
    root = tree.getroot()

    output_lines = []
    prev_line_was_heading = False

    # Process all paragraphs
    for p in root.findall('.//w:p', NAMESPACES):
        lines = process_paragraph(p, {}, rels)

        for line in lines:
            # Avoid excessive newlines
            if line.strip() == '':
                if output_lines and output_lines[-1].strip() != '':
                    output_lines.append('')
            else:
                output_lines.append(line)

        prev_line_was_heading = lines and lines[0].startswith('#')

    # Clean up excessive blank lines
    cleaned_lines = []
    prev_empty = False
    for line in output_lines:
        if line.strip() == '':
            if not prev_empty:
                cleaned_lines.append('')
            prev_empty = True
        else:
            cleaned_lines.append(line)
            prev_empty = False

    # Write output
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(cleaned_lines))

    print(f"Converted to {output_path}")


if __name__ == '__main__':
    base_dir = r"D:\github\2-2-STM32\STM32\Projects\Robot2026\Guo\材料提交"
    unpacked_dir = os.path.join(base_dir, 'unpacked')

    xml_path = os.path.join(unpacked_dir, 'word', 'document.xml')
    rels_path = os.path.join(unpacked_dir, 'word', '_rels', 'document.xml.rels')
    output_path = os.path.join(base_dir, '参考材料.md')

    convert_docx_xml_to_md(xml_path, output_path, rels_path)
