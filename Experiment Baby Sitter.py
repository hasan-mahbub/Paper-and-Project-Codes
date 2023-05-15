import glob
import numbers
import os
from time import sleep
import wmi
import time

prev_files = 0
start = time.time()
for i in range(1000):
    f = wmi.WMI()
    all_process = ' '
    print("All Process name")
    for process in f.Win32_Process():
        print(f"{process.ProcessId:<10} {process.Name}")
        all_process += process.Name
 
    
    if 'chi660d' in all_process :
        ff=time.time()-start
        ff=ff/3600
        from email import parser
        import requests
        payload = '{"text": "Software is running(Hours):"}'
        response = requests.post(
        'https://hooks.slack.com/services/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',
        data = payload)
        print(response.text)
        payload =  {'text': '{:.2f}'.format(ff)}
        payload = str(payload)
        import requests
        response = requests.post(
        'URL',
        data = payload)
        print(response.text)
        sleep(5)
        
        filelist = os.listdir(r'C:\Users\nelab\Desktop\Projects\4.Feb\Microfluid 2\EIS')
        current_files= len(filelist)
        print(current_files, '<-------------------------------')
        
        if current_files - prev_files >= 1:
            from email import parser
            import requests
            payload = '{"text": "Number of files saved:"}'
            response = requests.post(
            'URL',
            data = payload)
            payload =  {'text': '{}'.format(current_files)}
            payload = str(payload)
            print(payload,'<----------------------')
            import requests
            response = requests.post(
            'URL',
            data = payload)
            print(response.text)
            prev_files = current_files

        else:
            from email import parser
            import requests
            payload = '{"text": "Files are not getting saved "}'
            response = requests.post(
            'URL',
            data = payload)
            print(response.text)
            prev_files = current_files

    else:
        from email import parser
        import requests
        payload = '{"text": "Software has been terminated"}' 
        response = requests.post(
        'URL',
        data = payload)
        print(response.text)
        sleep(5)

    sleep(3600)
