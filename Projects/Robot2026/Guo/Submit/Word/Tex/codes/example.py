"""示例：数据处理脚本"""

from pathlib import Path
from typing import List


def parse_hex_line(line: str) -> int:
    """将一行十六进制字符串转为整数。"""
    return int(line.strip(), 16)


def load_values(path: Path) -> List[int]:
    with path.open(encoding="utf-8") as f:
        return [parse_hex_line(line) for line in f if line.strip()]


if __name__ == "__main__":
    data = load_values(Path("input.txt"))
    print(f"共读取 {len(data)} 条记录，首项 = {data[0:#x}")
