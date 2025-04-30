import sys
import time
import logging
import os
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from colorama import Fore, Style, init

# Initialize colorama for Windows color support
init()

class PythonScriptHandler(FileSystemEventHandler):
    def __init__(self, script_name=None):
        self.script_name = script_name
        self.last_run_time = 0

    def on_modified(self, event):
        if event.is_directory:
            return

        # Get file extension
        _, ext = os.path.splitext(event.src_path)

        # Only process .py files
        if ext.lower() != '.py':
            return

        # If a specific script is targeted, only process that one
        if self.script_name and os.path.basename(event.src_path) != self.script_name:
            return

        # Debounce - prevent multiple runs when editor saves multiple times
        current_time = time.time()
        if current_time - self.last_run_time < 2:  # 2 seconds debounce
            return
        self.last_run_time = current_time

        print(f"\n{Fore.GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━")
        print(f"{Fore.YELLOW}🔄 File changed: {Fore.CYAN}{event.src_path}{Style.RESET_ALL}")
        print(f"{Fore.GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━{Style.RESET_ALL}\n")

        # Run the Python script
        try:
            print(f"{Fore.YELLOW}▶️ Running: {Fore.CYAN}python {event.src_path}{Style.RESET_ALL}\n")
            subprocess.run([sys.executable, event.src_path], check=True)
            print(f"\n{Fore.GREEN}✅ Script completed successfully{Style.RESET_ALL}")
        except subprocess.CalledProcessError as e:
            print(f"\n{Fore.RED}❌ Script failed with error code {e.returncode}{Style.RESET_ALL}")
        except Exception as e:
            print(f"\n{Fore.RED}❌ Error: {e}{Style.RESET_ALL}")

def watch_python_files(path='.', script_name=None):
    event_handler = PythonScriptHandler(script_name)
    observer = Observer()
    observer.schedule(event_handler, path, recursive=True)
    observer.start()

    script_msg = f" (watching {script_name})" if script_name else ""
    print(f"{Fore.CYAN}Python file watcher started{script_msg}.{Style.RESET_ALL}")
    print(f"{Fore.CYAN}Watching for changes in .py files in {path} and its subdirectories.{Style.RESET_ALL}")
    print(f"{Fore.YELLOW}Press Ctrl+C to stop.{Style.RESET_ALL}")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='Watch and automatically run Python files when they change.')
    parser.add_argument('--script', '-s', help='Watch a specific Python script only')
    parser.add_argument('--path', '-p', default='.', help='Directory to watch (default: current directory)')
    args = parser.parse_args()

    watch_python_files(args.path, args.script)
