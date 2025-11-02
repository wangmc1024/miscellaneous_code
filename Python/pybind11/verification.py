import pybind11
import sysconfig

# 打印pybind11版本
print(f"pybind11 version: {pybind11.__version__}")

# 获取pybind11的头文件路径
print(f"pybind11 include path: {pybind11.get_include()}")

# 获取Python的头文件路径（使用sysconfig）
print(f"python include path: {sysconfig.get_path('include')}")
