import argparse
import urllib.request
import zipfile
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


if not os.path.exists("steamcache"):
    os.makedirs("steamcache")
    url = "https://steamcdn-a.akamaihd.net/client/installer/steamcmd.zip"
    file_name = "./steamcache/steamcmd.zip"
    # 下载文件
    urllib.request.urlretrieve(url, file_name)
    # 解压缩文件
    with zipfile.ZipFile(file_name, 'r') as zip_ref:
        zip_ref.extractall("./steamcache")
    os.chdir("./steamcache")
    print('SFSFSFS')
    print(os.path.abspath("./steamcache"))
    if sys.platform == "win32":
        os.system("SteamCmd.exe " + win)
    elif sys.platform == "linux":
        os.system("SteamCmd.exe " + linux)
    else:
        print("Unknown system.")
