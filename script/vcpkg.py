# BEGIN: c1d8v5hj8x9p
import argparse
import os
import json
import subprocess
import hashlib

# 创建参数解析器
parser = argparse.ArgumentParser(description="Python脚本示例")

# 添加-d参数
parser.add_argument("-d", "--directory", help="指定工作目录")

# 解析命令行参数
args = parser.parse_args()

# 如果提供了-d参数，则设置工作目录
if args.directory:
    new_dir = args.directory
    os.chdir(new_dir)
    print("Updated working directory:", os.getcwd())
else:
    print("No working directory parameter provided.")

# 读取vcpkg.json文件中的dependencies数组
with open('vcpkg.json', 'r') as f:
    data = json.load(f)
dependencies = data['dependencies']
os.chdir("..")

# 计算vcpkg.json的hash
hash_md5 = hashlib.md5(json.dumps(
    data, sort_keys=True).encode('utf-8')).hexdigest()

# 读取缓存文件中的hash
cache_file = os.path.join(os.getcwd(), new_dir+'/out/vcpkg_cache.txt')
if os.path.exists(cache_file):
    with open(cache_file, 'r') as f:
        cache_hash = f.read().strip()
else:
    cache_hash = ''

# 如果hash不同则打印生成，并把hash保存进缓存文件
if hash_md5 != cache_hash:
    cmd = new_dir+'/ThirdTool/vcpkg/vcpkg.exe'
    print(cmd)
    print("Generating vcpkg dependencies...")
    for dep in dependencies:
        subprocess.run([cmd,
                       'install', dep+":x64-windows-static"])
    with open(cache_file, 'w') as f:
        f.write(hash_md5)
# END: c1d8v5hj8x9p
