# WebSocket Json Demo - javascript Client / C++ Server 



- 編譯檔:

  | OS        | Compiler        | 檔案              | 備註                                          |
  | --------- | --------------- | ----------------- | --------------------------------------------- |
  | Windows10 | Mingw64 (MSYS2) | compile_win.bat   | 需要連結 ws2_32.a , wsock32.a  , 且要 -static |
  | MacOSX    | Clang++         | compile_mac.sh    | 不需要 -static                                |
  | Ubuntu    | GNU g++         | compile_ubuntu.sh | 需要 -pthread                                 |
  |           |                 |                   |                                               |

- 實作參考:

  - https://github.com/adamrehn/websocket-server-demo

- 第三方專案 (thirdparty):

  - json for modern c++  https://github.com/nlohmann/json

  - websocketpp  https://github.com/zaphoyd/websocketpp 

  - Asio https://think-async.com/Asio/

- 程式碼詳見:
  - Server: jsonserver.cpp (websocket_server.hpp) 
  - Client :  jsonclient.html 



