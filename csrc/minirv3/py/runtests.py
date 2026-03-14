import subprocess
import os
import sys

# 核心逻辑：遍历tests下的*_tests文件，执行并检查结果
print("Running unit tests:")

# 获取VALGRIND环境变量（和原shell脚本一致）
valgrind = os.environ.get("VALGRIND", "")

# 遍历tests目录下所有*_tests文件
for filename in os.listdir("tests"):
    # 只处理*_tests的文件
    if filename.endswith("_tests"):
        test_path = os.path.join("tests", filename)
        # 检查是否是文件（对应原shell的test -f $i）
        if os.path.isfile(test_path):
            # 构造执行命令（带valgrind或直接执行）
            cmd = f"{valgrind} ./{test_path}" if valgrind else f"./{test_path}"
            # 执行命令，重定向stderr到tests.log（对应2>>）
            with open("tests/tests.log", "a") as f:
                result = subprocess.run(
                    cmd,
                    shell=True,
                    stderr=f,  # 标准错误输出到日志文件
                    stdout=subprocess.PIPE,  # 捕获标准输出（可选）
                    text=True  # 以文本模式处理输出
                )
            
            # 检查执行结果（对应原shell的$? -eq 0）
            if result.returncode == 0:
                print(f"{test_path} PASS")
            else:
                print(f"ERROR in test {test_path}: here's tests/tests.log")
                print("------")
                # 打印日志最后几行（对应tail tests/tests.log）
                with open("tests/tests.log", "r") as f:
                    # 取最后10行，和tail默认行为一致
                    lines = f.readlines()
                    print(''.join(lines[-10:]))
                sys.exit(1)

print("")