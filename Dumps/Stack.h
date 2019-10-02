

#pragma once

#include <cstddef>
#include <utility>
#include <memory>
#include <iostream>
#include <cassert>
#include <fstream>
#include <typeinfo>
#include <type_traits>
#include <cxxabi.h>




#define VERIFED(code) Dump(code, __FILE__, __LINE__, __FUNCTION__) || (({assert(false);}), false)

#define VARIABLE_NAME(name) GetVariableName(#name)


enum ERRORS {
    NO_ERRORS,
    CANARY_IS_NOT_VALID,
    HASHES_DO_NOT_MATCH,
    POISONS_IS_NOT_VALID,
    THIS_IS_A_NULLPTR
};




template <typename T>
class DefaultPoison {
public:
    const int static POISON = (T)(0xCACED426);
};



template <typename T, typename Poison = DefaultPoison<T>, class Hash = std::hash<T>, size_t n = 10>
class UnkillableStack {
private:
    int canary_in_beginning = Poison::POISON;

    T* buffer_;
    int buffer_size_;
    int top_;
    int real_size_;

    Poison poison_;
    Hash hash_;
    size_t checksum_ = 0;
    std::string name_;
    int canary_in_end = Poison::POISON;

    bool IsCanaryValid();
    bool IsHashValid();
    bool IsOtherCellsValid();
    size_t GetErrorNumber();

    int Dump(size_t code, const char* file_name, size_t line, const char* function_name);

    void ReallocateBuffer();

    size_t GetHash();


public:
    UnkillableStack(std::string name);

    ~UnkillableStack();

    template<typename ...Args>
    void Push(Args &&... args);

    void PrintData() {
        for (size_t i = 0; i < buffer_size_; ++i) {
            std::cout << "buffer_[" << i << "] = " << buffer_[i] << std::endl;
        }
        std::cout << std::endl;
    }

    void Pop();
};







template <typename T, typename Poison, class Hash, size_t n>
UnkillableStack <T, Poison, Hash, n>::UnkillableStack(std::string name)
        : buffer_((T*)new char [sizeof(T) * (n + 2)])
        , buffer_size_(n + 2)
        , top_(1)
        , real_size_(0)
        , name_(name)

{
            for (size_t i = 0; i < n + 2; ++i) {
                buffer_[i] = Poison::POISON;
            }
            checksum_ = GetHash();
}

template <typename T, typename P, class H, size_t n>
UnkillableStack<T, P, H, n>::~UnkillableStack() {
    delete[] buffer_;
}

template <typename T, typename Poison, class Hash, size_t n>
template <typename ...Args>
void UnkillableStack<T, Poison, Hash, n>::Push(Args&&... args) {
    VERIFED(GetErrorNumber());
    if (buffer_size_ - real_size_ <= 3) {
        ReallocateBuffer();
    }
    new(buffer_ + top_) T(std::forward<Args>(args)...);
    real_size_ += 1;
    top_ += 1;
    checksum_ = GetHash();
    VERIFED(GetErrorNumber());
}

template <typename T, typename Poison, class Hash, size_t n>
void UnkillableStack<T, Poison, Hash, n>::Pop() {
    VERIFED(GetErrorNumber());
    real_size_ -= 1;    assert(real_size_ >= 0);
    top_ -= 1;  assert(top_ >= 0);
    buffer_[top_] = poison_();
    checksum_ = GetHash();
    VERIFED(GetErrorNumber());
}

template <typename T, typename P, class H, size_t n>
size_t UnkillableStack<T, P, H, n> ::GetHash() {
    size_t res_hash = 0;
    size_t power = 1;
    size_t p = 7;
    for (size_t i = 1; i < real_size_; ++i) {
        res_hash += hash_(buffer_[i]) * power;
        power *= p;
    }
    return res_hash;
}

template <typename T, typename P, class H, size_t n>
bool UnkillableStack<T, P, H, n>::IsCanaryValid() {
    return (buffer_[0] == poison_() && buffer_[buffer_size_ - 1] == poison_()
    && canary_in_beginning == Poison::POISON && canary_in_end == Poison::POISON);
}

template <typename T, typename P, class H, size_t n>
bool UnkillableStack<T, P, H, n>::IsHashValid() {
    return checksum_ == GetHash();
}

template <typename T, typename P, class H, size_t n>
bool UnkillableStack<T, P, H, n>::IsOtherCellsValid() {
    for (size_t i = top_; i < buffer_size_ - 1; ++i) {
        if (buffer_[i] != poison_()) return false;
    }
    return true;
}

template <typename T, typename P, class H, size_t n>
size_t UnkillableStack<T, P, H, n>::GetErrorNumber() {
    if (this == nullptr) {
        return THIS_IS_A_NULLPTR;
    }
    if (!IsCanaryValid()) {
        return ERRORS::CANARY_IS_NOT_VALID;
    }
    if (!IsHashValid()) {
        return ERRORS::HASHES_DO_NOT_MATCH;
    }
    if (!IsOtherCellsValid()) {
        return ERRORS::POISONS_IS_NOT_VALID;
    }
    return ERRORS::NO_ERRORS;
}

template <typename T, typename P, class H, size_t n>
void UnkillableStack<T, P, H, n>::ReallocateBuffer() {
    size_t new_size = buffer_size_ * 2;
    T* new_buffer = (T*)(new char[sizeof(T) * new_size]);
    for (size_t i = 0; i < buffer_size_; ++i) {
        new (new_buffer + i) T(buffer_[i]);
    }
    for (size_t i = buffer_size_; i < new_size; ++i) {
        new (new_buffer + i) T(poison_());
    }
    delete[] buffer_;
    buffer_ = new_buffer;
    buffer_size_ = new_size;
}

inline std::string GetVariableName(std::string name) {
    return name;
}

template <typename T, typename P, class H, size_t n>
inline std::string Demangle(UnkillableStack<T, P, H, n>* ptr);

template <typename T, typename P, class H, size_t n>
std::string Demangle(UnkillableStack<T, P, H, n>* ptr) {
    int status = -1;
    std::string realname = abi::__cxa_demangle(typeid(T()).name(), nullptr, nullptr, &status);
    return (status == 0) ? realname : typeid(T()).name();
}

template <typename T, typename P, class H, size_t n>
int UnkillableStack<T, P, H, n>::Dump(size_t code, const char *file_name, size_t line, const char *function_name) {
    if (code == NO_ERRORS) return 1;
    std::string error;
    if (code == CANARY_IS_NOT_VALID) {
        error = "CANARY_IS_NOT_VALID";
    }
    if (code == HASHES_DO_NOT_MATCH) {
        error = "HASHES_DO_NOT_MATCH";
    }
    if (code == POISONS_IS_NOT_VALID) {
        error = "POISONS_IS_NOT_VALID";
    }
    if (code == THIS_IS_A_NULLPTR) {
        error = "THIS_IS_A_NULLPTR";
    }

    std::fstream out;
    out.open("../dump", std::fstream::out);

    out << "Error checking is FAILED!!!" << " From PATH:" << file_name << " (" << line << ") " << "in " <<
    function_name << "()" << std::endl;

    out << "UnkillableStack<T> " << name_ << " with T = " << Demangle(this) <<
        " [" << this << "]" << std::endl;

    out << "{" << std::endl;
    out << "ERROR CODE = " << code << " (" << error << ")" << std::endl;
    out << "BUFFER_SIZE = " << buffer_size_ << ", REALSIZE = " << real_size_ << std::endl;
    out << "BUFFER [" << buffer_size_ << "] [" << buffer_ << "]" << std::endl;
    out << "{" << std::endl;
    out << "  [0]" << buffer_[0] << "  (POISON)" << std::endl;


    for (size_t i = 1; i <= real_size_; ++i) {
        out << "* [" << i << "] = " << buffer_[i] << std::endl;
    }
    for (size_t i = std::max(1, real_size_ + 1); i < buffer_size_; ++i) {
        out << "  [" << i << "] = " << buffer_[i] << "  (POISON)" << std::endl;
    }
    out << "}" << std::endl;
    out << "}" << std::endl;
    out.close();
    return 0;

}



