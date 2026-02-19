/*
 *  Copyright (c) 2026, the Omnia developers
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#if __has_include(<features.h>)
#    include <features.h>
#endif

#ifndef USING_OA_GLOBALLY
#    define USING_OA_GLOBALLY 1
#endif

#ifdef __x86_64__
#    define OA_IS_ARCH_X86_64() 1
#else
#    define OA_IS_ARCH_X86_64() 0
#endif

#ifdef __aarch64__
#    define OA_IS_ARCH_AARCH64() 1
#else
#    define OA_IS_ARCH_AARCH64() 0
#endif

#if (defined(__SIZEOF_POINTER__) && __SIZEOF_POINTER__ == 8)
#    define OA_ARCH_64_BIT
#else
#    define OA_ARCH_32_BIT
#endif

#if defined(__clang__) || defined(__CLION_IDE__) || defined(__CLION_IDE_)
#    define OA_COMPILER_CLANG
#elif defined(__GNUC__)
#    define OA_COMPILER_GCC
#endif

#if defined(COMPILER_CLANG) && defined(__apple_build_version__)
#    define OA_COMPILER_APPLE_CLANG
#endif

#if defined(__GLIBC__)
#    define OA_LIBC_GLIBC
#    define OA_LIBC_GLIBC_PREREQ(maj, min) __GLIBC_PREREQ((maj), (min))
#else
#    define OA_LIBC_GLIBC_PREREQ(maj, min) 0
#endif

#if defined(__linux__)
#    define OA_OS_LINUX
#endif

#if defined(__APPLE__) && defined(__MACH__) && !defined(__IOS__)
#    define OA_OS_MACOS
#    define OA_OS_BSD_GENERIC
#endif

#if defined(__IOS__)
#    define OA_OS_IOS
#    define OA_OS_BSD_GENERIC
#endif

#if defined(__MACH__)
#    define OA_OS_MACH
#endif

#define ARCH(arch) (OA_IS_ARCH_##arch())

#ifdef ALWAYS_INLINE
#    undef ALWAYS_INLINE
#endif
#define ALWAYS_INLINE __attribute__((always_inline)) inline

#ifdef NEVER_INLINE
#    undef NEVER_INLINE
#endif
#define NEVER_INLINE __attribute__((noinline))

#ifdef COLD
#    undef COLD
#endif
#define COLD __attribute__((cold))

#ifdef HOT
#    undef HOT
#endif
#define HOT __attribute__((hot))

#ifdef FLATTEN
#    undef FLATTEN
#endif
#define FLATTEN __attribute__((flatten))

#ifdef RETURNS_NONNULL
#    undef RETURNS_NONNULL
#endif
#define RETURNS_NONNULL __attribute__((returns_nonnull))

#ifdef NO_SANITIZE_COVERAGE
#    undef NO_SANITIZE_COVERAGE
#endif
#if defined(COMPILER_CLANG)
#    define NO_SANITIZE_COVERAGE __attribute__((no_sanitize("coverage")))
#else
#    define NO_SANITIZE_COVERAGE __attribute__((no_sanitize_coverage))
#endif

#ifdef NO_SANITIZE_ADDRESS
#    undef NO_SANITIZE_ADDRESS
#endif
#if defined(COMPILER_CLANG)
#    define NO_SANITIZE_ADDRESS __attribute__((no_sanitize("address")))
#else
#    define NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#endif

#ifdef DISALLOW
#    undef DISALLOW
#endif
#if defined(OA_COMPILER_CLANG)
#    define DISALLOW(message) __attribute__((diagnose_if(1, message, "error")))
#else
#    define DISALLOW(message) __attribute__((error(message)))
#endif

#ifdef NO_UNIQUE_ADDRESS
#    undef NO_UNIQUE_ADDRESS
#endif

#if defined(LIFETIME_BOUND)
#    undef LIFETIME_BOUND
#endif
#if defined(COMPILER_CLANG)
#    define LIFETIME_BOUND [[clang::lifetimebound]]
#else
#    define LIFETIME_BOUND
#endif

#ifndef __has_feature
#    define __has_feature(...) 0
#endif

#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#    define HAS_ADDRESS_SANITIZER
#    define ASAN_POISON_MEMORY_REGION(addr, size) __asan_poison_memory_region(addr, size)
#    define ASAN_UNPOISON_MEMORY_REGION(addr, size) __asan_unpoison_memory_region(addr, size)
#    define LSAN_REGISTER_ROOT_REGION(base, size) __lsan_register_root_region(base, size)
#    define LSAN_UNREGISTER_ROOT_REGION(base, size) __lsan_unregister_root_region(base, size)
#    define LSAN_IGNORE_OBJECT(base) __lsan_ignore_object(base)
#else
#    define ASAN_POISON_MEMORY_REGION(addr, size)
#    define ASAN_UNPOISON_MEMORY_REGION(addr, size)
#    define LSAN_REGISTER_ROOT_REGION(base, size)
#    define LSAN_UNREGISTER_ROOT_REGION(base, size)
#    define LSAN_IGNORE_OBJECT(base)
#endif

#if __has_feature(blocks)
#    define OA_HAS_BLOCKS
#endif

#if __has_feature(objc_arc)
#    define OA_HAS_OBJC_ARC
#endif

#ifdef OA_OS_WINDOWS
#    define PAGE_SIZE 4096
#else
#    include <unistd.h>
#    undef PAGE_SIZE
#    define PAGE_SIZE sysconf(_SC_PAGESIZE)
#endif

#ifndef OA_SYSTEM_CACHE_ALIGNMENT_SIZE
#    if ARCH(AARCH64) || ARCH(X86_64)
#        define OA_SYSTEM_CACHE_ALIGNMENT_SIZE 64
#    else
#        define OA_SYSTEM_CACHE_ALIGNMENT_SIZE 128
#    endif
#endif

#ifdef OA_CACHE_ALIGNED
#    undef OA_CACHE_ALIGNED
#endif
#define OA_CACHE_ALIGNED alignas(OA_SYSTEM_CACHE_ALIGNMENT_SIZE)
