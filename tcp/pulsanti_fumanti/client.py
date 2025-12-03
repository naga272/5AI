import socket
import sys

HOST = "127.0.0.1"
PORT = 5000


def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    while True:
        data = s.recv(1024)
        if not data:
            break

        msg = data.decode()
        sys.stdout.write(msg)

        if msg.startswith("NOME:"):
            name = input()
            s.sendall(name.encode())
            continue

        if msg.startswith("SCRIVI:"):
            txt = input()
            s.sendall(txt.encode())


if __name__ == "__main__":
    main()
