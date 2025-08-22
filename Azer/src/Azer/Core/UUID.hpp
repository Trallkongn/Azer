// uuid.hpp ¡ª header-only UUID v4 (RFC 4122) for C++17
// Usage:
//   #include "uuid.hpp"
//   UUID id = UUID::GenerateV4();
//   std::string s = id.ToString();
//   UUID x; if (UUID::TryParse(s, x)) { /* ok */ }
//   std::unordered_map<UUID, int> map; // hash supported
//
// Notes:
// - Version 4 (random). Sets version/variant bits per RFC 4122.
// - Uses std::random_device to seed a thread_local PRNG, then fills 16 bytes.
// - Header-only. No deps. C++17.

#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <charconv>
#include <algorithm>
#include <random>
#include <mutex>
#include <stdexcept>
#include <functional>

struct UUID {
    std::array<std::uint8_t, 16> bytes{};

    // --- Constructors ---
    UUID() = default;
    explicit UUID(const std::array<std::uint8_t, 16>& b) : bytes(b) {}

    // Nil UUID (all zeros)
    static UUID Nil() { return UUID{}; }

    bool IsNil() const {
        for (auto b : bytes) if (b != 0) return false;
        return true;
    }

    // --- Generation (v4 random) ---
    static UUID GenerateV4() {
        auto& eng = prng();
        std::array<std::uint8_t, 16> b{};
        // Fill with random bytes
        for (size_t i = 0; i < 16; i += 8) {
            auto v = std::uniform_int_distribution<std::uint64_t>()(eng);
            for (int k = 0; k < 8; ++k) b[i + k] = static_cast<std::uint8_t>((v >> (k * 8)) & 0xFF);
        }
        // Set version (4) and variant (10x)
        b[6] = static_cast<std::uint8_t>((b[6] & 0x0F) | 0x40); // version 4
        b[8] = static_cast<std::uint8_t>((b[8] & 0x3F) | 0x80); // variant RFC 4122
        return UUID{ b };
    }

    // --- String conversion ---
    std::string ToString(bool uppercase = false) const {
        static constexpr char hex_lo[] = "0123456789abcdef";
        static constexpr char hex_hi[] = "0123456789ABCDEF";
        const char* hex = uppercase ? hex_hi : hex_lo;
        std::string s;
        s.resize(36);
        size_t j = 0;
        for (size_t i = 0; i < 16; ++i) {
            if (i == 4 || i == 6 || i == 8 || i == 10) s[j++] = '-';
            s[j++] = hex[(bytes[i] >> 4) & 0x0F];
            s[j++] = hex[(bytes[i]) & 0x0F];
        }
        return s;
    }

    static bool TryParse(std::string_view str, UUID& out) {
        // Accept forms: 8-4-4-4-12 (36 chars) optionally with braces { } (38)
        if (str.size() == 38 && str.front() == '{' && str.back() == '}') {
            str = str.substr(1, 36);
        }
        if (str.size() != 36) return false;
        auto is_hex = [](char c) {
            return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
            };
        // Hyphens at 8,13,18,23
        if (str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-') return false;
        std::array<std::uint8_t, 16> b{};
        size_t bi = 0;
        for (size_t i = 0; i < 36; ) {
            if (str[i] == '-') { ++i; continue; }
            if (i + 1 >= 36 || !is_hex(str[i]) || !is_hex(str[i + 1])) return false;
            auto hv = [](char c)->int {
                if (c >= '0' && c <= '9') return c - '0';
                if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
                return 10 + (c - 'A');
                };
            int hi = hv(str[i]);
            int lo = hv(str[i + 1]);
            b[bi++] = static_cast<std::uint8_t>((hi << 4) | lo);
            i += 2;
        }
        if (bi != 16) return false;
        out = UUID{ b };
        return true;
    }

    // Comparisons
    friend bool operator==(const UUID& a, const UUID& b) { return a.bytes == b.bytes; }
    friend bool operator!=(const UUID& a, const UUID& b) { return !(a == b); }
    friend bool operator<(const UUID& a, const UUID& b) { return a.bytes < b.bytes; }

    // Version helper (only v4 supported by generator)
    int Version() const { return (bytes[6] >> 4) & 0x0F; }
    int Variant() const { return (bytes[8] >> 6); } // should be 2 for RFC 4122

private:
    // Thread-safe PRNG seeded from random_device once per thread
    static std::mt19937_64& prng() {
        thread_local std::mt19937_64 eng = [] {
            std::random_device rd;
            std::seed_seq seed{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
            return std::mt19937_64(seed);
            }();
        return eng;
    }
};

// Hash support for unordered_map / unordered_set
namespace std {
    template<> struct hash<UUID> {
        size_t operator()(const UUID& u) const noexcept {
            // FNV-1a 64-bit over 16 bytes
            const uint64_t FNV_OFFSET = 1469598103934665603ull;
            const uint64_t FNV_PRIME = 1099511628211ull;
            uint64_t h = FNV_OFFSET;
            for (auto b : u.bytes) { h ^= b; h *= FNV_PRIME; }
            return static_cast<size_t>(h);
        }
    };
}

// --- Optional convenience: operator<< for streams ---
#include <ostream>
inline std::ostream& operator<<(std::ostream& os, const UUID& id) {
    return os << id.ToString();
}
