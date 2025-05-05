# LokiRAT

**LokiRAT** es un proyecto avanzado que implementa un **Troyano de Acceso Remoto (RAT)** desde cero. No es una herramienta básica: está diseñado para demostrar técnicas reales empleadas en malware moderno, con un enfoque en funcionalidades potentes y evasivas.

El objetivo principal es **comprender a fondo cómo operan las RAT reales**, tanto desde una perspectiva de **seguridad ofensiva** (pentesting/red teaming) como de **seguridad defensiva** (análisis de malware, threat hunting, ingeniería de detección). Entender su funcionamiento interno te permite desarrollar mejores mecanismos de defensa.

![aa](https://github.com/user-attachments/assets/afdd7fb7-d7e6-4793-9954-cfa162cd071e)

---

## 📘 Aprende con el Curso

Cada funcionalidad implementada en LokiRAT se explica paso a paso en el curso:

**[Construyendo un Troyano de Acceso Remoto Personalizado](https://0x12darkdev.net/courses/building-a-custom-remote-access-trojan-rat)**

### Aprenderás a:

- Diseñar y desarrollar tu propio RAT desde cero.  
- Aplicar técnicas de evasión y ocultación utilizadas por malware avanzado.  
- Construir una infraestructura completa de Comando y Control (C2).  
- Comprender el funcionamiento interno de Windows, hooking de API y desarrollo de rootkits.  
- Implementar comunicación robusta entre cliente y servidor (C++ ↔ C2).

> **Nota:** Aunque se incluye un panel web hecho en Angular (LokiWebViewer), el curso no cubre fundamentos de desarrollo web. El foco está exclusivamente en el desarrollo de malware.

---

## 🛠️ Funcionalidades

### Anti-Detección & Evasión

- **TimeStomping:** modifica timestamps para parecer legítimo.  
- **Unhook NTDLL:** reemplaza la NTDLL modificada por una limpia para evadir hooks.  
- **Parcheo de ETW:** neutraliza el registro en tiempo real de eventos.  
- **Sin nuevos hilos:** ejecuta shellcode sin crear nuevos threads.  
- **Module Stomping:** ejecuta payload en memoria de módulos legítimos.  
- Enmascaramiento de procesos con comandos aparentes.  
- Reutilización de handles legítimos para ocultar acciones maliciosas.

### Persistencia y Escalada

- **Ejecución como Administrador:** evade UAC para elevar privilegios.  
- **Creación de tareas programadas** para persistencia.  
- **Escalada a SISTEMA:** mediante robo de tokens.  
- **Recopilación de info del sistema, procesos y red.**  
- **Detección de software de seguridad instalado.**

### Rootkit

- **Userland Rootkit:** intercepta llamadas API.  
- **Ocultador de archivos, procesos y claves del registro.**  
- **Restaurador de archivos ocultos.**

### Manejo de Archivos

- **Carga y descarga de archivos** fragmentados vía HTTP.  
- **Explorador de archivos** remoto.

### Keylogging

- **Registro de teclas** en segundo plano.

### RDP y Credenciales

- **RDP Stealer:** extracción de sesiones y credenciales guardadas.

---

## ⚙️ Tecnologías Utilizadas

LokiRAT se compone de tres módulos principales:

### 🔹 Loki (Agente / Cliente)

- **Lenguaje:** C++  
- **Plataforma:** Windows  
- **Propósito:** Payload ejecutado en la máquina objetivo.  
- **Destacado:** Uso de API nativa, evasión, manipulación de memoria y anti-análisis.

### 🔸 LokiServer (Servidor C2)

- **Lenguaje:** C# (.NET)  
- **Framework:** ASP.NET Core  
- **Propósito:** Servidor de Comando y Control con API REST.  
- **Destacado:** Seguridad basada en tokens, gestión de agentes, comandos en tiempo real.

### 🔺 LokiWebViewer (Panel Web)

- **Lenguaje:** TypeScript  
- **Framework:** Angular  
- **Propósito:** Interfaz web para gestionar agentes infectados vía API C2.  
- **Destacado:** UI limpia, visualización de datos, ejecución remota de comandos.

---

## ⚠️ Aviso Legal y Ético

Este proyecto es **exclusivamente educativo**. Está destinado al estudio, análisis y desarrollo de defensas ante malware avanzado.

**No lo uses con fines maliciosos.** El uso indebido de esta herramienta para atacar sistemas sin autorización es ilegal y va en contra de los principios éticos de la ciberseguridad.

Cumple siempre con las leyes locales y sigue las mejores prácticas del sector.

---

## Créditos

Este proyecto y curso fueron desarrollados por [0x12darkdev](https://0x12darkdev.net/).  
Todo el contenido es propiedad de su autor original y se presenta aquí únicamente con fines educativos y de documentación.
