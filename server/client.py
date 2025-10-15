import socket
import sys


HOST = '127.0.0.1'  # Server's address (localhost in this case)
PORT = 9999        # Server's port


def main(fd) -> int:
    data = fd.recv(1024).decode('utf-8')
    print(data)

    while 1:
        message = input("Scrivi al servo: ").strip()

        fd.sendall(message.encode('utf-8'))

        try:
            data = fd.recv(1024).decode('utf-8')

            if data:
                print(f"Ricevuto dal server: {data}")

            if data.upper() == 'Hai indovinato!'.upper():
                fd.close()
                return 0

        except Exception as err:
            print("Connessione chiusa dal servo", err)
            return 1


if __name__ == "__main__":
    fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    fd.connect((HOST, PORT))
    print(f"Connected to server at {HOST}:{PORT}")

    result = main(fd)

    print(f"uscito dal programma con valore {result}")
    sys.exit(result)
