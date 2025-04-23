# POCO Crypto 模块详解

`Crypto` 模块提供了常见的密码学操作功能，如哈希摘要、加密解密、数字签名和随机数生成。**需要注意的是，POCO 的 Crypto 模块通常依赖于底层的 OpenSSL 库来实现这些功能。** 因此，在使用此模块前，需要确保 OpenSSL 开发库已正确安装并在 POCO 构建时启用。

## 主要功能

1.  **摘要算法 (Hashing/Digests):**
    *   `DigestEngine`: 摘要计算引擎的基类。
    *   `MD5Engine`, `SHA1Engine`, `SHA256Engine`, `SHA512Engine` 等: 提供了常用的哈希算法实现。可以计算数据流或字节序列的摘要值。
    *   `HMACEngine`: 用于计算基于哈希的消息认证码 (HMAC)。
2.  **加密/解密 (Ciphers):**
    *   `CipherFactory`, `Cipher`, `CipherKey`: 提供了对称加密和解密框架。
    *   支持常见的对称加密算法，如 AES (Advanced Encryption Standard), DES (Data Encryption Standard), Triple DES 等，以及不同的操作模式（如 ECB, CBC, OFB 等）和填充方案。
    *   `RSA`: 提供了 RSA 非对称加密/解密和签名/验证功能 (通常直接使用 OpenSSL 的 RSA API 封装)。
3.  **随机数生成 (Random Numbers):**
    *   `RandomStream`: 提供了一个用于生成密码学安全伪随机数的流接口。
    *   `Random`: 提供了生成随机字节序列的功能。
4.  **数字签名与验证:**
    *   结合摘要引擎和非对称加密（如 RSA）来实现数字签名和验证。
    *   `RSADigestEngine`: 方便地将摘要计算和 RSA 签名/验证结合起来。
5.  **X.509 证书处理:**
    *   `X509Certificate`: 提供了加载、解析和验证 X.509 数字证书的功能。

## 设计特点

*   **封装 OpenSSL:** 主要目的是提供一个更 C++ 化、更易用的接口来访问 OpenSSL 的密码学功能。
*   **流式接口:** 摘要计算和随机数生成支持流式操作。
*   **工厂模式:** `CipherFactory` 用于根据算法名称创建对应的 `Cipher` 实例。

## 关键组件关系示例 (AES 加密)

以下 Mermaid 图示意了使用 Crypto 模块进行 AES 对称加密的基本过程：

```mermaid
graph TD
    subgraph 应用程序
        A[应用程序代码]
        B[明文数据 (流/字符串)]
        C[密钥 (CipherKey)]
        D[初始化向量 (IV), 如果需要]
        E[密文输出 (流/字符串)]
    end

    subgraph POCO Crypto
        F{CipherFactory} -- "获取实例" --> A;
        F -- "创建" --> G[Cipher (AES)];
        G -- "初始化 (加密模式, 密钥, IV)" --> C;
        G -- "初始化 (加密模式, 密钥, IV)" --> D;
        H(CryptoOutputStream) -- "包装" --> E;
        H -- "持有" --> G;
    end

    subgraph OpenSSL (底层)
        I[OpenSSL EVP API]
        J[OpenSSL AES 实现]
    end

    A -- "1. 获取 CipherFactory" --> F;
    A -- "2. 创建 Cipher (e.g., AES-256-CBC)" --> F;
    A -- "3. 创建 CipherKey" --> C;
    A -- "4. (可选) 创建 IV" --> D;
    A -- "5. 初始化 Cipher (加密模式)" --> G;
    A -- "6. 创建 CryptoOutputStream" --> H;
    A -- "7. 将明文写入流" --> B;
    B -- "写入" --> H;
    H -- "调用 Cipher 加密" --> G;
    G -- "通过 OpenSSL EVP" --> I;
    I -- "调用具体实现" --> J;
    J -- "返回加密块" --> I;
    I -- "返回给" --> G;
    G -- "写入加密块到" --> H;
    H -- "写入底层流" --> E;
    A -- "8. 关闭 CryptoOutputStream (处理填充)" --> H;

```

**流程说明:**

1.  获取 `CipherFactory` 实例。
2.  使用工厂的 `createCipher()` 方法，传入算法名称（如 "aes-256-cbc"）来创建 `Cipher` 对象。
3.  创建 `CipherKey` 对象，包含对称密钥。对于某些模式（如 CBC），还需要提供初始化向量 (IV)。
4.  调用 `Cipher` 对象的 `encrypt()` 或相关初始化方法，传入密钥、IV（如果需要）和加密模式。
5.  (推荐方式) 创建一个 `Poco::Crypto::CryptoOutputStream`，将它绑定到一个输出流（用于接收密文），并传入配置好的 `Cipher` 对象。
6.  将明文数据写入 `CryptoOutputStream`。
7.  `CryptoOutputStream` 在内部调用 `Cipher` 对数据进行加密，并将加密后的数据写入到底层输出流。
8.  关闭 `CryptoOutputStream` 时，会处理必要的填充并刷新缓冲区。

解密过程类似，只是在初始化 `Cipher` 时选择解密模式，并使用 `CryptoInputStream` 来读取密文并自动解密。

Crypto 模块为需要安全功能的 C++ 应用程序提供了重要的基础支持。 