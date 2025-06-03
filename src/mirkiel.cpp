#include "mirkiel.h"

#include <fstream>
#include <iostream>
#include <vector>
#include "utils.h"
#include "scanner.h"
#include "token.h"
namespace mirkiel {
    void MirKiel::runFile(const std::filesystem::path& path_) {
        auto bytes = readFile<std::vector<uint8_t>>(path_);
        std::string content(bytes.begin(), bytes.end());
        run(content);
        if (m_hadError)
            util::systemExit(65);
    }

    void MirKiel::runPrompt() {
        std::string line;

        for (;;) {
            std::cout << "> ";

            if (!std::getline(std::cin, line)) {
                std::cout << std::endl;
                break;
            }

            if (line == "exit" || line == "quit") {
                break;
            }
            run(line);
            m_hadError = false;
        }
    }
    void MirKiel::run(const std::string& src_) {
        Scanner scanner(src_);
        std::vector<Token> tokens = scanner.scanTokens();

        for (auto&& token : tokens) {
           std::cout << token << "\n";
        }
    }

    void MirKiel::error(int line_, std::string msg_) {
        report(line_, "", msg_);
    }

    MirKiel::MirKiel() {}

    MirKiel::~MirKiel() {}

    void MirKiel::report(int line_, std::string where_, std::string msg_) {
        std::cout << "[Line " + std::to_string(line_) + "] Error" + where_ + "; " + msg_ << "\n";
         m_hadError = true;
    }

    template <typename Container>
    Container readFile(const std::filesystem::path& path_) {
        std::ifstream file(path_, std::ios::binary);
        Container result;

        if (!file.is_open()) {
            std::cout << "Failed to open " << path_.string() << '\n';
            return result;
        }

        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        if constexpr (requires(Container c, size_t n) { c.resize(n); }) {
            result.resize(fileSize);

            file.read(reinterpret_cast<char*>(result.data()), fileSize);
        } else {
            char buffer[1024];
            while (file.read(buffer, sizeof(buffer))) {
                result.insert(result.end(), buffer, buffer + file.gcount());
            }
            if (file.gcount() > 0) {
                result.insert(result.end(), buffer, buffer + file.gcount());
            }
        }

        return result;
    }

}  // namespace Mirkiel
