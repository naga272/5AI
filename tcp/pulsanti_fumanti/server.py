import socket
import threading
import random
import time

HOST = "0.0.0.0"
PORT = 5000

TARGET_POINTS = 5
WORDS = ["alfa", "bravo", "charlie", "delta", "echo", "foxtrot",
         "kilo", "lambda", "omega", "python", "socket", "network"]

clients = {}
scores = {}
lock = threading.Lock()


def broadcast(msg):
    for c in clients.values():
        try:
            c.sendall(msg.encode())
        except Exception:
            pass


def handle_client(conn, addr):
    conn.sendall(b"NOME: ")
    name = conn.recv(1024).decode().strip()

    with lock:
        clients[name] = conn
        scores[name] = 0

    while True:
        pass


def game_loop():
    while True:
        if len(clients) < 3:
            time.sleep(1)
            continue

        broadcast("PARTITA START\n")

        while True:
            target = random.choice(WORDS)
            t_msg = f"SCRIVI: {target}\n"
            broadcast(t_msg)

            winner = None
            start = time.time()

            while winner is None:
                for name, conn in list(clients.items()):
                    conn.setblocking(False)
                    try:
                        data = conn.recv(1024)
                    except Exception:
                        continue
                    if not data:
                        continue

                    if data.decode().strip() == target:
                        winner = name
                        break

            elapsed = time.time() - start

            with lock:
                scores[winner] += 1
                sc = "\n".join([f"{p}: {v}" for p, v in scores.items()])

            broadcast(f"\nVINCITORE: {winner}  (+1)\nTempo: {elapsed:.3f}s\n{sc}\n")

            if scores[winner] >= TARGET_POINTS:
                broadcast(f"\nFINE PARTITA — Vincitore: {winner}\n")
                for c in clients.values():
                    c.close()
                clients.clear()
                scores.clear()
                break


def main():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((HOST, PORT))
    s.listen()

    threading.Thread(target=game_loop, daemon=True).start()

    while True:
        conn, addr = s.accept()
        threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()


if __name__ == "__main__":
    main()
