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
