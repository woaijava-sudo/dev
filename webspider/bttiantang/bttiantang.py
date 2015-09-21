import httplib
import urlparse
import urllib
import re

'''
Downloader
author: bajdcc
host: bttiantang
usage: link -> input
'''

def download():
    url_origin = raw_input('input link:\n')
    url = urlparse.urlparse(url_origin)
    params = urlparse.parse_qs(url.query, True)
    headers = {
        'User-Agent' : 'Mozilla/5.0 (Windows NT 6.3; WOW64) \
                        AppleWebKit/537.36 (KHTML, like Gecko) \
                        Chrome/42.0.2311.152 Safari/537.36',
        'Accept'     : 'text/html,application/xhtml+xml,\
                        application/xml;q=0.9,*/*;q=0.8',
        'Connection' : 'close',
        'Accept-Language' : 'zh-CN,zh;q=0.8,en;q=0.6,zh-TW;q=0.4',
        'Accept-Charset'  : 'GBK,utf-8;q=0.7,*;q=0.3',
        'Content-Type': 'application/x-www-form-urlencoded',
        'Refer' : 'http://www.bttiantang.com/download.php',
        'Origin' : 'http://www.bttiantang.com/download.php'
    }
    values = {
        'action' : 'download',
        'id' : params['id'][0],
        'uhash' : params['uhash'][0],
        'imageField.x' : '81',
        'imageField.y' : '20',
    }
    conn = httplib.HTTPConnection('www.bttiantang.com')
    data = urllib.urlencode(values)
    data = data.encode('utf-8')
    conn.request('POST', '/download1.php', data, headers)

    try:
        response = conn.getresponse()
        file_name = response.getheader('Content-Disposition').decode('utf8')
        pattern = re.compile('"(.*)"')
        file_name = pattern.findall(file_name)[0]
        file_data = response.read()
        with open(file_name, 'wb') as f:
            f.write(file_data)
        print 'saving to %s' % file_name
    except httplib.HTTPException as e:
        print ('http_error: ', e.code)

    conn.close()
    return


if __name__ == '__main__':
    download()
