# lbyarch_program

**Specs:**

![image](https://github.com/user-attachments/assets/3bee97a8-9686-485a-bcd4-b45c3fecd2e8)

![image](https://github.com/user-attachments/assets/23c8ef71-0ff4-4942-a18b-e032e2c66513)


**Command Prompt Compilation & Execution:**

  nasm -f win64 saxpy_test_asm.asm
  
  gcc -c saxpy_test_c.c -o saxpy_test_c.obj -m64
  
  gcc saxpy_test_c.obj saxpy_test_asm.obj -o saxpy_test_c.exe -m64
  
  saxpy_test_c.exe


**Comparative Analysis:**

Testing between the C kernel and x86-64 assembly using large vector sizes reveals significant differences in execution time, with C generally being slower due to its higher-level abstraction. The C kernel simplifies development by automatically handling memory management, function calls, and other complex operations, making it portable across different platforms. However, this ease of use comes at the cost of performance, as the compiler must translate C code into machine instructions, which introduces additional overhead. In contrast, x86-64 assembly allows for direct control over the hardware, enabling precise optimization at the instruction level. This results in faster execution, as developers can take full advantage of the processor’s capabilities, such as SIMD instructions and efficient register usage. While x86-64 assembly requires more complex coding and lacks portability, its ability to improve performance makes it significantly faster than the C kernel, especially in performance-critical applications where execution time is important.

**Manual Input:**

![image](https://github.com/user-attachments/assets/c13806dd-e273-4a1b-b545-8120035e4b8c)


**Execution time of vector size n = {2^20, 2^24, and 2^28}:**

![image](https://github.com/user-attachments/assets/b4e8f82a-50b3-4f79-b97c-93914983f6ff)

![image](https://github.com/user-attachments/assets/b2472b98-9698-446c-8bfd-6043ca17782c)

![image](https://github.com/user-attachments/assets/30d072d1-f980-4350-b458-35396028d3c8)
