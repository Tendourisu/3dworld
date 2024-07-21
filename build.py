import glob, os

if not os.path.exists("build"):
    os.mkdir("build")

cpp_files = glob.glob("**/*.cpp", recursive=True)

os.system("g++ " + " ".join(cpp_files) + " -o build/main")