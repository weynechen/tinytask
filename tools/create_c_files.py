import os

def create_files(names):
    for name in names:
        # 确保名称有效，避免文件系统不允许的字符
        safe_name = ''.join([c for c in name if c.isalnum() or c in ['_', '-']])
        
        # 创建 .c 文件
        c_file_path = f"{safe_name}.c"
        with open(c_file_path, 'w') as c_file:
            #c_file.write(f"// {safe_name}.c file\n")
            c_file.write(f"#include \"{safe_name}.h\"\n\n")
        
        # 创建 .h 文件
        h_file_path = f"{safe_name}.h"
        with open(h_file_path, 'w') as h_file:
            #h_file.write(f"// {safe_name}.h file\n")
            h_file.write(f"#ifndef __{safe_name.upper()}_H__\n")
            h_file.write(f"#define __{safe_name.upper()}_H__\n\n")
            h_file.write(f"#endif // __{safe_name.upper()}_H__\n")

# 从命令行参数获取名称列表
if __name__ == "__main__":
    import sys
    # 忽略脚本名称，只获取名称参数
    names = sys.argv[1:]
    
    if not names:
        print("请提供至少一个名称。")
    else:
        create_files(names)
        print(f"已成功创建以下文件：{', '.join(names)}")