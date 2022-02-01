import time

start_time = time.time()

def msg(text: str) -> None:
    start = f"FF {(time.time() - start_time):.2f}".ljust(11)
    print(f"{start} {text}")

def die(text: str) -> None:
    msg(f"ERROR: {text}")
    exit(1)

