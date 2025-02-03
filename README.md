# 🌐 CWEB - The Minimalist C Web Server 🚀

Welcome to **CWEB**, the lightweight, high-performance web server built entirely in **C**! Say goodbye to the bloat and complexity of Apache and Nginx. With CWEB, you can serve your HTML pages with ease, speed, and simplicity. 🛠️

---

## 🚀 **Features**

- **Lightning Fast** ⚡: Built in C for maximum performance.
- **Minimalist** 🎯: No unnecessary bloat, just what you need to serve web pages.
- **Easy to Use** 🛠️: Simple configuration and setup.
- **Cross-Platform** 🌍: Runs on Linux, macOS, and Windows (with minimal adjustments).
- **Open Source** 💻: Fully open-source and customizable.

---

## 📦 **Installation**

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/AhqafCoder/cweb.git
   cd cweb
   ```

2. **Compile the Server**:
   ```bash
   gcc -o server server.c
   ```

3. **Run the Server**:
   ```bash
   ./server
   ```

4. **Access Your Web Page**:
   Open your browser and navigate to `http://localhost:8080`.

---

## 🛠️ **Configuration**

CWEB is designed to be simple. Just edit the `server.c` file to configure the port, root directory, and other settings.

```c
#define PORT 8080
#define ROOT_DIR "www"
```

---

## 📂 **Directory Structure**

```
CWEB/
├── server.c          # Main server code
├── www/            # Root directory for your HTML files
│   ├── index.html  # Your main HTML file
│   └── style.css   # Optional CSS file
└── README.md       # This file
```

---

## 🚨 **Limitations**

- **Basic Functionality**: CWEB is designed for simplicity and may not support advanced features like SSL, virtual hosts, or complex routing.
- **Single-Threaded**: Currently, CWEB handles one request at a time. Multi-threading support is planned for future versions.

---

## 🛠️ **Future Plans**

- **Multi-Threading Support** 🧵: Handle multiple requests simultaneously.
- **SSL/TLS Support** 🔒: Secure your web server with HTTPS.
- **Configuration File** 📄: External configuration file for easier setup.
- **Dynamic Content Support** 🎨: Add support for server-side scripting.

---

## 🤝 **Contributing**

We welcome contributions! If you have any ideas, bug fixes, or improvements, feel free to open an issue or submit a pull request.

1. Fork the repository.
2. Create a new branch 
3. Commit your changes 
4. Push to the branch
5. Open a pull request.

---

## 📜 **License**

CWEB is licensed under the **MIT License**. Feel free to use, modify, and distribute it as you see fit.

---

## 🙏 **Acknowledgements**

- **Inspired by**: The simplicity and power of C.
- **Special Thanks**: To the open-source community for endless inspiration.

---

## 📞 **Contact**

Have questions or need help? Feel free to reach out!

- **Email**: ahqafaliofficial@gmail.com
- **Twitter**: -
- **GitHub**: [AhqafCoder](https://github.com/AhqafCoder)

---

## 🌟 **Star This Repository**

If you find CWEB useful, please give it a ⭐️ on GitHub! Your support helps us improve and grow.

---

**Happy Coding!** 🎉  
**- The CWEB Team** 🚀