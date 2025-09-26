#include <iostream>
#include <filesystem>
#include <string>
#include <regex>
#include <io.h>
#include <fcntl.h>

namespace fs = std::filesystem;

int main() {
    // set console to UTF-8 for proper Unicode output
    (void)_setmode(_fileno(stdout), _O_U8TEXT);

    std::wregex pattern(LR"(^\d{1,20}-(.+)$)");
    int renamedCount = 0;

    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            if (entry.is_regular_file()) {
                std::wstring oldName = entry.path().filename().wstring();
                std::wsmatch match;

                if (std::regex_match(oldName, match, pattern)) {
                    std::wstring newName = match[1].str();
                    fs::path newPath = entry.path().parent_path() / newName;

                    fs::rename(entry.path(), newPath);

                    // show old file name → new file name
                    std::wcout << oldName << L" → " << newName << std::endl;

                    renamedCount++;
                }
            }
        }

        if (renamedCount == 0) {
            std::wcout << L"No files were renamed." << std::endl;
        }
        else {
            std::wcout << L"\nTotal files renamed: " << renamedCount << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::wcerr << L"Error: " << ex.what() << std::endl;
    }

    std::wcout << L"\nPress Enter to exit...";
    std::wcin.get();
    return 0;
}
