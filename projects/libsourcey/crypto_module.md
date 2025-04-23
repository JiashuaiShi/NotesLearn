# 加密模块 (`src/crypto`)

## 概览

`crypto` 模块封装了底层的 OpenSSL 库，提供了常用的加密操作接口，包括对称加密、哈希计算、HMAC、RSA 加密/签名以及 X.509 证书处理。

## 关键组件与概念

*   **头文件位置:** `src/crypto/include/scy/crypto/`
*   **依赖:** `scy/base`, `OpenSSL`

*   **初始化 (`crypto.h`):**
    *   `initializeEngine()` / `uninitializeEngine()`: 全局初始化/反初始化 OpenSSL 库，建议在程序启动/退出时调用。
    *   `ByteVec`: `std::vector<unsigned char>` 的类型别名，用于存储二进制数据。

*   **对称加密 (`cipher.h`):**
    *   **`Cipher` 类:** 封装 OpenSSL EVP 接口，提供对称加密/解密。
        *   支持多种算法 (取决于 OpenSSL)。
        *   支持多种密钥/IV 设置方式：直接设置、随机生成、从密码派生。
        *   支持流式 (`update`/`final`) 和一次性 (`encryptString`/`decryptString`, `encryptStream`/`decryptStream`) 操作。
        *   支持不同输出编码 (Binary, Base64, BinHex)。
        *   可控制填充 (`setPadding`)。

*   **哈希计算 (`hash.h`):**
    *   (推测) 提供常用哈希算法 (MD5, SHA1, SHA256 等) 的计算接口。可能包含一个 `Hasher` 类或类似的自由函数。

*   **HMAC (`hmac.h`):**
    *   (推测) 提供基于哈希的消息认证码 (HMAC) 计算功能，可能依赖 `hash.h`。

*   **RSA (`rsa.h`):**
    *   (推测) 提供 RSA 非对称加密/解密和签名/验签的功能。可能包含处理 RSA 密钥对的类。

*   **X.509 证书 (`x509certificate.h`):**
    *   (推测) 提供加载、解析和验证 X.509 证书的功能。这对于 SSL/TLS 通信 (`net` 模块) 非常重要。

## 设计理念

*   **封装 OpenSSL:** 将复杂的 OpenSSL C API 封装成易于使用的 C++ 类。
*   **面向对象:** 为不同的加密操作 (Cipher, Hash, HMAC, RSA, Certificate) 提供专门的类。
*   **资源管理:** C++ 类通过构造/析构函数管理 OpenSSL 对象的生命周期。
*   **易用性:** 提供便捷函数 (如 `encryptString`) 和流式接口简化常见任务。

## 与其他模块关系

*   **`net` 模块:** `SSLSocket` 强依赖此模块进行 SSL/TLS 握手、证书验证和数据加密。`sslcontext.h`, `sslmanager.h` 等可能直接使用了 `crypto` 模块的功能，特别是 `X509Certificate`。 