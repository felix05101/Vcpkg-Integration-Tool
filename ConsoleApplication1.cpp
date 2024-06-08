#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <direct.h>
#include <Windows.h>

namespace fs = std::filesystem;

// Function to check if a directory exists
bool directoryExists(const std::string& dir) {
    DWORD attrib = GetFileAttributesA(dir.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

// Function to check if Git is installed by attempting to run a Git command
bool isGitInstalled() {
    std::string command = "git --version";
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        return false;
    }
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    _pclose(pipe);
    return result.find("git version") != std::string::npos;
}

// Function to install Git using shell command
void installGit(const std::string& currentUserDir) {
    std::string gitInstallerUrl = "https://github.com/git-for-windows/git/releases/download/v2.45.2.windows.1/Git-2.45.2-64-bit.exe";
    std::string gitInstallerPath = currentUserDir + "\\GitInstaller.exe";

    // Download Git installer
    std::string downloadCommand = "powershell -command \"(New-Object System.Net.WebClient).DownloadFile('" + gitInstallerUrl + "', '" + gitInstallerPath + "')\"";
    system(downloadCommand.c_str());

    // Install Git
    std::string installCommand = "\"" + gitInstallerPath + "\" /SILENT /NORESTART";
    system(installCommand.c_str());

    // Clean up
    fs::remove(gitInstallerPath);
}

// Function to download and install Vcpkg using Git
void installVcpkg(const std::string& currentUserDir) {
    std::string vcpkgRepoUrl = "https://github.com/microsoft/vcpkg.git";
    std::string vcpkgDir = currentUserDir + "\\vcpkg";

    // Clone Vcpkg repository using Git
    std::string cloneCommand = "git clone " + vcpkgRepoUrl + " " + vcpkgDir;
    system(cloneCommand.c_str());

    // Bootstrap Vcpkg
    std::string bootstrapCommand = vcpkgDir + "\\bootstrap-vcpkg.bat";
    system(bootstrapCommand.c_str());
}

// Function to integrate Vcpkg with Visual Studio
void integrateVcpkgWithVisualStudio(const std::string& vcpkgDir) {
    std::string integrateCommand = vcpkgDir + "\\vcpkg integrate install";
    system(integrateCommand.c_str());
}

int main() {
    // Get the current user's directory
    std::string currentUserDir = getenv("USERPROFILE");
    std::string vcpkgDir = currentUserDir + "\\vcpkg";

    // Check if Git is installed
    if (!isGitInstalled()) {
        std::cout << "Git is not installed. Installing..." << std::endl;
        installGit(currentUserDir);
    }
    else {
        std::cout << "Git is already installed." << std::endl;
    }

    // Check if Vcpkg is installed
    if (!directoryExists(vcpkgDir)) {
        std::cout << "Vcpkg is not installed. Installing..." << std::endl;
        installVcpkg(currentUserDir);
    }

    // Change directory to vcpkg directory
    int result = _chdir(vcpkgDir.c_str());
    if (result != 0) {
        std::cerr << "Failed to change directory to vcpkg directory." << std::endl;
        return 1;
    }

    // Integrate Vcpkg with Visual Studio
    integrateVcpkgWithVisualStudio(vcpkgDir);

    std::cout << "Setup complete. Vcpkg is now integrated with Visual Studio." << std::endl;

    // Loop to accept user input commands
    std::string command;
    while (true) {
        std::cout << "\n";
        std::cout << "Enter a command ('install libraryname', 'delete libraryname', 'exit' to quit): ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }
        else if (command.find("install ") == 0) {
            std::string libraryName = command.substr(8); // Extract library name after "install "
            std::string installCommand = ".\\vcpkg install " + libraryName;
            std::cout << "Executing command: " << installCommand << std::endl;
            system(installCommand.c_str());
        }
        else if (command.find("delete ") == 0) {
            std::string libraryName = command.substr(7); // Extract library name after "delete "
            std::string deleteCommand = ".\\vcpkg remove " + libraryName;
            std::cout << "Executing command: " << deleteCommand << std::endl;
            system(deleteCommand.c_str());

            // Clean up residual files after deletion
            std::string packageDir = ".\\vcpkg\\packages\\" + libraryName + "*";
            for (const auto& entry : fs::directory_iterator(packageDir)) {
                std::cout << "Removing residual file: " << entry.path() << std::endl;
                fs::remove_all(entry.path());
            }
        }
        else {
            std::cerr << "Invalid command." << std::endl;
        }
    }

    return 0;
}
