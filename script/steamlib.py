import urllib.request
import zipfile
import io
import os
import sys

win = "+@ShutdownOnFailedCommand 1 "\
    "+@NoPromptForPassword 1 "\
    "+@sSteamCmdForcePlatformType windows "\
    "+login anonymous "\
    "+force_install_dir ../ "\
    "+app_update 1007 validate "\
    "+quit"
linux = "+@ShutdownOnFailedCommand 1 "\
    "+@NoPromptForPassword 1 "\
    "+@sSteamCmdForcePlatformType linux "\
    "+login anonymous "\
    "+force_install_dir ../ "\
    "+app_update 1007 validate "\
    "+quit"


import argparse
import os

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
    print("更新后的工作目录:", os.getcwd())
else:
    print("没有提供工作目录参数。")



if not os.path.exists("steam"):
    os.makedirs("steam")
    url = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip"
    file_name = "./steam/steamcmd.zip"
    # 下载文件
    urllib.request.urlretrieve(url, file_name)
    # 解压缩文件
    with zipfile.ZipFile(file_name, 'r') as zip_ref:
        zip_ref.extractall("./steam")


# 执行 steam目录下SteamCmd.exe +runscript ../sdk_win.txt

os.chdir("./steam")
print('SFSFSFS')
print(os.path.abspath("./steam"))
# # 执行 SteamCmd.exe +runscript ../sdk_linux.txt
if sys.platform == "win32":
    os.system("SteamCmd.exe " + win)
elif sys.platform == "linux":
    os.system("SteamCmd.exe " + linux)
else:
    print("Unknown system.")
