@echo off
echo Installing required packages for live reload if not already installed...
pip install watchdog

echo.
echo Starting Python file watcher for monty_python_trump_roast.py
echo.
echo This watcher will automatically re-run the script when you save changes
echo.
echo TO STOP THE WATCHER: Press Ctrl+C, then press Y when prompted
echo.

python -c "
import time
import os
import subprocess
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class ScriptHandler(FileSystemEventHandler):
    def on_modified(self, event):
        if event.src_path.endswith('monty_python_trump_roast.py'):
            print('\n\n--- File changed: %s ---' % os.path.basename(event.src_path))
            print('--- Re-running script ---\n')
            subprocess.call(['python', 'monty_python_trump_roast.py'])

observer = Observer()
path = '.'
event_handler = ScriptHandler()
observer.schedule(event_handler, path, recursive=False)
observer.start()

print('Watching for changes to monty_python_trump_roast.py...')
print('Initial run of script:')
subprocess.call(['python', 'monty_python_trump_roast.py'])

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    observer.stop()
observer.join()
"

pause
