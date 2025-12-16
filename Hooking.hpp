#pragma once

#include "pch.h"

#include <vector>

#include "logging.hpp"
#include "Sig.hpp"

const std::vector<uint8_t> PROLOGUE = {
    0x50,                                     // push rax
    0x53,                                     // push rbx
    0x51,                                     // push rcx
    0x52,                                     // push rdx
    0x56,                                     // push rsi
    0x57,                                     // push rdi
    0x55,                                     // push rbp
    0x41, 0x50,                               // push r8
    0x41, 0x51,                               // push r9
    0x41, 0x52,                               // push r10
    0x41, 0x53,                               // push r11
    0x41, 0x54,                               // push r12
    0x41, 0x55,                               // push r13
    0x41, 0x56,                               // push r14
    0x41, 0x57,                               // push r15
    0x9c,                                     // pushf
    0x48, 0x81, 0xec, 0x00, 0x01, 0x00, 0x00, // sub rsp,0x100
    0xf3, 0x0f, 0x7f, 0x04, 0x24,             // movdqu XMMWORD PTR [rsp],xmm0
    0xf3, 0x0f, 0x7f, 0x4c, 0x24, 0x10,       // movdqu XMMWORD PTR [rsp+0x10],xmm1
    0xf3, 0x0f, 0x7f, 0x54, 0x24, 0x20,       // movdqu XMMWORD PTR [rsp+0x20],xmm2
    0xf3, 0x0f, 0x7f, 0x5c, 0x24, 0x30,       // movdqu XMMWORD PTR [rsp+0x30],xmm3
    0xf3, 0x0f, 0x7f, 0x64, 0x24, 0x40,       // movdqu XMMWORD PTR [rsp+0x40],xmm4
    0xf3, 0x0f, 0x7f, 0x6c, 0x24, 0x50,       // movdqu XMMWORD PTR [rsp+0x50],xmm5
    0xf3, 0x0f, 0x7f, 0x74, 0x24, 0x60,       // movdqu XMMWORD PTR [rsp+0x60],xmm6
    0xf3, 0x0f, 0x7f, 0x7c, 0x24, 0x70,       // movdqu XMMWORD PTR [rsp+0x70],xmm7
    0xf3, 0x44, 0x0f, 0x7f, 0x84, 0x24, 0x80, // movdqu XMMWORD PTR [rsp+0x80],xmm8
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0x8c, 0x24, 0x90, // movdqu XMMWORD PTR [rsp+0x90],xmm9
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0x94, 0x24, 0xa0, // movdqu XMMWORD PTR [rsp+0xa0],xmm10
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0x9c, 0x24, 0xb0, // movdqu XMMWORD PTR [rsp+0xb0],xmm11
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0xa4, 0x24, 0xc0, // movdqu XMMWORD PTR [rsp+0xc0],xmm12
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0xac, 0x24, 0xd0, // movdqu XMMWORD PTR [rsp+0xd0],xmm13
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0xb4, 0x24, 0xe0, // movdqu XMMWORD PTR [rsp+0xe0],xmm14
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x7f, 0xbc, 0x24, 0xf0, // movdqu XMMWORD PTR [rsp+0xf0],xmm15
    0x00, 0x00, 0x00,
    0x48, 0x89, 0xe3,                         // mov rbx,rsp
    0x48, 0x83, 0xe4, 0xf0,                   // and rsp,0xfffffffffffffff0
    0x48, 0x83, 0xec, 0x20,                   // sub rsp,0x20
};

const std::vector<uint8_t> EPILOGUE = {
    0x48, 0x89, 0xdc,                         // mov rsp,rbx
    0xf3, 0x44, 0x0f, 0x6f, 0xbc, 0x24, 0xf0, // movdqu xmm15,XMMWORD PTR [rsp+0xf0]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0xb4, 0x24, 0xe0, // movdqu xmm14,XMMWORD PTR [rsp+0xe0]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0xac, 0x24, 0xd0, // movdqu xmm13,XMMWORD PTR [rsp+0xd0]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0xa4, 0x24, 0xc0, // movdqu xmm12,XMMWORD PTR [rsp+0xc0]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0x9c, 0x24, 0xb0, // movdqu xmm11,XMMWORD PTR [rsp+0xb0]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0x94, 0x24, 0xa0, // movdqu xmm10,XMMWORD PTR [rsp+0xa0]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0x8c, 0x24, 0x90, // movdqu xmm9,XMMWORD PTR [rsp+0x90]
    0x00, 0x00, 0x00,
    0xf3, 0x44, 0x0f, 0x6f, 0x84, 0x24, 0x80, // movdqu xmm8,XMMWORD PTR [rsp+0x80]
    0x00, 0x00, 0x00,
    0xf3, 0x0f, 0x6f, 0x7c, 0x24, 0x70,       // movdqu xmm7,XMMWORD PTR [rsp+0x70]
    0xf3, 0x0f, 0x6f, 0x74, 0x24, 0x60,       // movdqu xmm6,XMMWORD PTR [rsp+0x60]
    0xf3, 0x0f, 0x6f, 0x6c, 0x24, 0x50,       // movdqu xmm5,XMMWORD PTR [rsp+0x50]
    0xf3, 0x0f, 0x6f, 0x64, 0x24, 0x40,       // movdqu xmm4,XMMWORD PTR [rsp+0x40]
    0xf3, 0x0f, 0x6f, 0x5c, 0x24, 0x30,       // movdqu xmm3,XMMWORD PTR [rsp+0x30]
    0xf3, 0x0f, 0x6f, 0x54, 0x24, 0x20,       // movdqu xmm2,XMMWORD PTR [rsp+0x20]
    0xf3, 0x0f, 0x6f, 0x4c, 0x24, 0x10,       // movdqu xmm1,XMMWORD PTR [rsp+0x10]
    0xf3, 0x0f, 0x6f, 0x04, 0x24,             // movdqu xmm0,XMMWORD PTR [rsp]
    0x48, 0x81, 0xc4, 0x00, 0x01, 0x00, 0x00, // add rsp,0x100
    0x9d,                                     // popf
    0x41, 0x5f,                               // pop r15
    0x41, 0x5e,                               // pop r14
    0x41, 0x5d,                               // pop r13
    0x41, 0x5c,                               // pop r12
    0x41, 0x5b,                               // pop r11
    0x41, 0x5a,                               // pop r10
    0x41, 0x59,                               // pop r9
    0x41, 0x58,                               // pop r8
    0x5d,                                     // pop rbp
    0x5f,                                     // pop rdi
    0x5e,                                     // pop rsi
    0x5a,                                     // pop rdx
    0x59,                                     // pop rcx
    0x5b,                                     // pop rbx
    0x58,                                     // pop rax
};

void* generateFullPatch(const std::vector<uint8_t>& patch, uint64_t returnAddr) {
    // jump to the final return addr
    uint8_t returnToEndAddr[] = {
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp QWORD PTR [rip]   
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // placeholder for address
    };
    memcpy(&returnToEndAddr[6], &returnAddr, 8);
    std::vector<uint8_t> returnToEndAddrVec(returnToEndAddr, returnToEndAddr + sizeof(returnToEndAddr));

    std::vector<uint8_t> fullCode;
    fullCode.reserve(PROLOGUE.size() + patch.size() + EPILOGUE.size() + returnToEndAddrVec.size());
    fullCode.insert(fullCode.end(), PROLOGUE.begin(), PROLOGUE.end());
    fullCode.insert(fullCode.end(), patch.begin(), patch.end());
    fullCode.insert(fullCode.end(), EPILOGUE.begin(), EPILOGUE.end());
    fullCode.insert(fullCode.end(), returnToEndAddrVec.begin(), returnToEndAddrVec.end());

    uint64_t memSize = fullCode.size();

    // copy it to static memory pages so it lives for the lifetime of the program
    void* newMem = VirtualAlloc(nullptr, memSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (newMem == nullptr) {
        return nullptr;
    }
    memcpy(newMem, fullCode.data(), memSize);

    DWORD oldProtect;
    VirtualProtect((void*)(newMem), memSize, PAGE_EXECUTE_READ, &oldProtect); // drv3 executable pages are E+R only by default
    return newMem;
}

void* findMemory(
    void* gameMemory,
    size_t gameMemoryLen,
    const char* signature,
    const char* mask
) {
    // game memory is decrypted in real time by Steam DRM, we need to loop the sigscan to wait until it's actually done
    int timesTried = 0;
    void* patchLocation = nullptr;
    do {
        patchLocation = (void*)Sig::find<Sig::Mask::Eq<'.'>, Sig::Mask::Any<'?'>>(gameMemory, gameMemoryLen, signature, mask);
        timesTried++;
        Sleep(500);
    } while (timesTried < 120 && patchLocation == nullptr); // try for 1 minute at most, something probably went wrong if it takes longer

    return patchLocation;
}

void writeMemory(void* location, const uint8_t* data, int size) {
    DWORD oldProtect;
    VirtualProtect((void*)(location), size, PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy((void*)(location), data, size);
    VirtualProtect((void*)(location), size, oldProtect, &oldProtect);
}

bool CodeHook(
    void* gameMemory,
    size_t gameMemoryLen,
    const char* signature, // patched area should be at least 14 bytes for the patch to fit correctly...
    const char* mask,
    const std::vector<uint8_t>& patch
) {

    void* patchLocation = findMemory(gameMemory, gameMemoryLen, signature, mask);
    if (patchLocation == nullptr) {
        return false;
    }

    uint64_t patchSize = strlen(mask);
    uint64_t returnAddr = ((uint64_t)patchLocation) + patchSize;
    void* fullPatchData = generateFullPatch(patch, returnAddr);
    if (fullPatchData == nullptr) {
        return false;
    }

    // creates the call to the full patch
    uint8_t callToRealPatch[] = {
        0xff, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp QWORD PTR [rip]   
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // placeholder for address
    };
    memcpy(&callToRealPatch[6], &fullPatchData, 8);
    
    // finally overwrite game memory
    std::vector<unsigned char> nopBuffer(patchSize, 0x90);
    writeMemory(patchLocation, nopBuffer.data(), patchSize); // fill with nops first
    writeMemory(patchLocation, callToRealPatch, sizeof(callToRealPatch)); // add patch now

    return true;
}