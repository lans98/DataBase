#pragma once

#include <iostream>

namespace error {

    using namespace std;

    enum ErrorKind : int {
        NULL_STORAGE,
        EMPTY_STORAGE, 
        INCORRECT_PARAMS,
        DUPLICATE_VALUE,
    };

    class Error {
    private:
        ErrorKind kind;
        string    message;

    public:
        Error() = delete;
        Error(const Error&) = default;
        Error(ErrorKind kind, string msg): kind(kind), message(msg) {}

        friend ostream& operator<<(ostream& out, const Error& error) {
            string kind_msg;

            switch (error.kind) {
                case NULL_STORAGE:
                    kind_msg = "NULL_STORAGE";
                    break;
                case EMPTY_STORAGE:
                    kind_msg = "EMPTY_STORAGE";
                    break;
                case INCORRECT_PARAMS:
                    kind_msg = "INCORRECT_PARAMS";
                    break;
                case DUPLICATE_VALUE:
                    kind_msg = "DUPLICATE_VALUE";
                    break;
                default:
                    kind_msg = "UNKNOWN_CODE";
                    break;
            }

            out << "Error { kind: \"" << kind_msg << "\", "
                << " code: " << error.kind  << ", "
                << " message: \"" << error.message << "\" }";

            return out;
        } 
    };
}
