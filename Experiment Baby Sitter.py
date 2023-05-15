import glob
import numbers
import os
from time import sleep
import wmi

prev_files = 0

for i in range(1000):
    f = wmi.WMI()
    all_process = ' '
    print("All Process name")
    for process in f.Win32_Process():
        print(f"{process.ProcessId:<10} {process.Name}")
        all_process += process.Name
 
    
    if 'firefox' in all_process :
        filelist = os.listdir(r'C:\Users\xsobu\OneDrive - George Mason University - O365 Production\Desktop\Python baby')
        current_files= len(filelist)
        print(current_files, '<-------------------------------')
        if current_files - prev_files >= 1:
            from email import parser
            import requests
            payload = '{"text": "Experiment is running"}'
            response = requests.post(
            'https://hooks.slack.com/services/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',
            data = payload)
            print(response.text)
            prev_files = current_files
    sleep(60)