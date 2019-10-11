
#pragma once

#include <cstddef>
#include <utility>
#include <memory>
#include <random>
#include <iostream>
#include <cassert>
#include <fstream>
#include <typeinfo>
#include <type_traits>
#include <cxxabi.h>



#define VERIFED(code) Dump(code, __FILE__, __LINE__, __FUNCTION__) || (({assert(false);}), false)

#define VARIABLE_NAME(name) GetVariableName(#name)




const int SHIFT = time(0) % 100;



enum ERRORS {
    NO_ERRORS,
    CANARY_IS_NOT_VALID,
    HASHES_DO_NOT_MATCH,
    POISONS_IS_NOT_VALID,
    POINTER_DESTRUCTED
};



template <typename T>
class DefaultPoison {
public:
    const int static POISON = T();
};

class IntViewer {
public:
    int operator() (int& elem) {
        return elem;
    }
};





template <typename T, class Viewer, typename Poison = DefaultPoison<T>, class Hash = std::hash<T>, size_t n = 10>
class UnkillableStack {
private:
    int canary_in_beginning = Poison::POISON;

    int buffer_size_;
    int top_;
    int real_size_;

    Hash hash_;
    Viewer viewer_;

    T* buffer_;

    size_t checksum_;
    std::string name_;

    int canary_in_end = Poison::POISON;

    bool IsCanaryValid();
    bool IsHashValid();
    bool IsOtherCellsValid();

    size_t GetErrorNumber();

    int Dump(size_t code, const char* file_name, size_t line, const char* function_name);

    void ReallocateBuffer();
    void StaticOutput(size_t code, const char *file_name, size_t line, const char *function_name, std::string error);

    size_t GetHash();
    T* GetLocation();


public:
    UnkillableStack(std::string name);

    UnkillableStack& operator= (const UnkillableStack& oth) = delete;

    ~UnkillableStack();

    template<typename ...Args>
    void EmplaceBack(Args &&... args);

    void Push(const T& elem);

    void Clear();

    T Top();

    void Pop();
};







template <typename T, class Viewer, typename Poison, class Hash, size_t n>
UnkillableStack <T, Viewer, Poison, Hash, n>::UnkillableStack(std::string name)
        : buffer_((T*)new char [sizeof(T) * (n + 2)])
        , buffer_size_(n + 2)
        , top_(1)
        , real_size_(0)
        , checksum_(0)
        , name_(name)

{

            for (size_t i = 0; i < n + 2; ++i) {
                buffer_[i] = Poison::POISON;
            }
            buffer_ = (T*)((size_t*)(buffer_) - SHIFT);
            checksum_ = GetHash();

}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
UnkillableStack<T, Viewer, Poison, Hash, n>::~UnkillableStack() {
    VERIFED(GetErrorNumber());
    delete[] GetLocation();

    buffer_ = nullptr;
}


template <typename T, class Viewer, typename Poison, class Hash, size_t n>
T UnkillableStack<T, Viewer, Poison, Hash, n>::Top() {
    assert(real_size_ >= 0);
    VERIFED(GetErrorNumber());
    return GetLocation()[top_ - 1];
}



template <typename T, class Viewer, typename Poison, class Hash, size_t n>
void UnkillableStack<T, Viewer, Poison, Hash, n>::Clear() {
    for (; real_size_;) {
        Pop();
    }
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
void UnkillableStack<T, Viewer, Poison, Hash, n>::Push(const T &elem) {
    VERIFED(GetErrorNumber());

    if (buffer_size_ - real_size_ <= 3) {
        ReallocateBuffer();
    }

    new(GetLocation() + top_) T(elem);

    real_size_ += 1;
    top_ += 1;
    checksum_ = GetHash();

    VERIFED(GetErrorNumber());
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
template <typename ...Args>
void UnkillableStack<T, Viewer, Poison, Hash, n>::EmplaceBack(Args&&... args) {

    VERIFED(GetErrorNumber());

    if (buffer_size_ - real_size_ <= 3) {
        ReallocateBuffer();
    }

    new(GetLocation() + top_) T(std::forward<Args>(args)...);

    real_size_ += 1;
    top_ += 1;
    checksum_ = GetHash();

    VERIFED(GetErrorNumber());
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
void UnkillableStack<T, Viewer, Poison, Hash, n>::Pop() {
    VERIFED(GetErrorNumber());

    real_size_ -= 1;    assert(real_size_ >= 0);
    top_ -= 1;  assert(top_ >= 0);

    (GetLocation() + top_)->~T();
    GetLocation()[top_] = Poison::POISON;

    checksum_ = GetHash();
    VERIFED(GetErrorNumber());
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
T* UnkillableStack<T, Viewer, Poison, Hash, n>::GetLocation() {
    return (T*)((size_t*)(buffer_) + SHIFT);
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
size_t UnkillableStack<T, Viewer, Poison, Hash, n> ::GetHash() {
    size_t old_checksum = checksum_;
    size_t res_hash = 0;
    checksum_ = 0;
    res_hash = std::hash<std::string>()(std::string(reinterpret_cast<const char*>(this),
            sizeof(UnkillableStack<T, Viewer, Poison, Hash, n>)));


    res_hash += std::hash<std::string>()(std::string(reinterpret_cast<const char*>(GetLocation()),
                                                    real_size_));
    checksum_ = old_checksum;
    return res_hash;
}



template <typename T, class Viewer, typename Poison, class Hash, size_t n>
bool UnkillableStack<T, Viewer, Poison, Hash, n>::IsCanaryValid() {
    return (GetLocation()[0] == Poison::POISON && GetLocation()[buffer_size_ - 1] == Poison::POISON
    && canary_in_beginning == Poison::POISON && canary_in_end == Poison::POISON);
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
bool UnkillableStack<T, Viewer, Poison, Hash, n>::IsHashValid() {
    size_t hash = GetHash();
    return (hash == checksum_);
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
bool UnkillableStack<T, Viewer, Poison, Hash, n>::IsOtherCellsValid() {
    if (!IsHashValid()) return false;
    for (size_t i = top_; i < buffer_size_ - 1; ++i) {
        if (GetLocation()[i] != Poison::POISON) return false;
    }
    return true;
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
size_t UnkillableStack<T, Viewer, Poison, Hash, n>::GetErrorNumber() {
    if (!IsHashValid()) {
        return ERRORS::HASHES_DO_NOT_MATCH;
    }
    if (this == nullptr || buffer_ == nullptr) {
        return POINTER_DESTRUCTED;
    }
    if (!IsCanaryValid()) {
        return ERRORS::CANARY_IS_NOT_VALID;
    }
    if (!IsOtherCellsValid()) {
        return ERRORS::POISONS_IS_NOT_VALID;
    }
    return ERRORS::NO_ERRORS;
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
void UnkillableStack<T, Viewer, Poison, Hash, n>::ReallocateBuffer() {
    size_t new_size = buffer_size_ * 2;

    T* new_buffer = (T*)(new char[sizeof(T) * new_size]);

    for (size_t i = 0; i < buffer_size_; ++i) {
        new (new_buffer + i) T(GetLocation()[i]);
    }

    for (size_t i = buffer_size_; i < new_size; ++i) {
        new (new_buffer + i) T(Poison::POISON);
    }
    delete[] GetLocation();

    buffer_ = new_buffer;
    buffer_ = (T*)((size_t*)(buffer_) - SHIFT);
    buffer_size_ = new_size;
}

inline std::string GetVariableName(std::string name) {
    return name;
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
inline std::string Demangle(UnkillableStack<T, Viewer, Poison, Hash, n>* ptr);

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
std::string Demangle(UnkillableStack<T, Viewer, Poison, Hash, n>* ptr) {
    int status = -1;
    std::string realname = abi::__cxa_demangle(typeid(T()).name(), nullptr, nullptr, &status);
    return (status == 0) ? realname : typeid(T()).name();
}

template <typename T, class Viewer, typename Poison, class Hash, size_t n>
void UnkillableStack<T, Viewer, Poison, Hash, n>::StaticOutput(size_t code, const char *file_name, size_t line,
                                                       const char *function_name, std::string error) {
    FILE* file = fopen("../dump", "w");
    fprintf(file, "Error checking is FAILED!!! From PATH: %s (%d) in %s() \n", file_name, line, function_name);
    std::fflush(file);
    if (error == "POINTER_DESTRUCTED") {
        fprintf(file, "POINTER_DESTRUCTED");
        fclose(file);
        return;
    }
    if (error == "HASHES_DO_NOT_MATCH") {
        fprintf(file, "HASHES_DO_NOT_MATCH");
        fclose(file);
        return;

    }
    fprintf(file, "UnkillableStack<T> %s with T = %s [%x] \n", name_.c_str(), Demangle(this).c_str(), this);
    std::fflush(file);
    fprintf(file, "{\nERROR CODE = %d (%s)\nBUFFER_SIZE = %d REALSIZE = %d\nBUFFER[%d][%x]\n{\n", code, error.c_str(),
            buffer_size_, real_size_, buffer_size_, GetLocation());
    std::fflush(file);
    fprintf(file, "  [0] = %d (POISON)\n", GetLocation()[0]);
    std::fflush(file);
    for (size_t i = 1; i <= real_size_; ++i) {
        fprintf(file, "* [%d] = %s\n", i, viewer_(GetLocation()[i]));
        std::fflush(file);
    }
    for (size_t i = std::max(1, real_size_ + 1); i < buffer_size_; ++i) {
        fprintf(file, "  [%d] = %d (POISON)\n", i, viewer_(GetLocation()[i]));
        std::fflush(file);
    }
    fprintf(file, "}\n}\n");
    std::fflush(file);
    fclose(file);
}


template <typename T, class Viewer, typename Poison, class Hash, size_t n>
int UnkillableStack<T, Viewer, Poison, Hash, n>::Dump(size_t code, const char *file_name, size_t line, const char *function_name) {
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
    if (code == POINTER_DESTRUCTED) {
        error = "POINTER_DESTRUCTED";
    } else{}

    StaticOutput(code, file_name, line, function_name, error);
    return 0;

}



