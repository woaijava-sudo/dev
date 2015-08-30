import sys
from download import dl
dl.delay(dict(url=sys.argv[1],name=sys.argv[2]))
