# download.py
import os
import urllib
from celery import Celery

celery = Celery('download', broker='redis://localhost:6379/0')
path=r'N:\TMP'

@celery.task(bind=True,acks_late=True,max_retries=3,default_retry_delay=1 * 60)
def dl(self,info,**kwargs):
    print('URL: %s' % info['url'])
    print('Name: %s' % info['name'])
    dst=os.path.join(path,info['name'])
    print('Location: %s' % dst)
    try:
        urllib.urlretrieve(info['url'], dst)        
    except Exception as exc:
        raise self.retry(exc=exc,countdown=20)
    else:
        print('Downloaded successfully.')
    finally:
        print('Exit.')
