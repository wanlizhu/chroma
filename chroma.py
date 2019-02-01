import sys
import getopt

def download(url, dir):
    return

class CommandDispatcher(object):
    def __init__(self):
        self.commands = {
            'sync' : self.sync,
            'build' : self.build,
            'demo' : self.demo,
        }

    def sync(self):
        return

    def build(self):
        return

    def demo(self):
        return


if __name__=='__main__':
    if len(sys.argv) <= 1:
        print('No valid command specified.')
        exit(1)

    dispatcher = CommandDispatcher()
    if sys.argv[1] in dispatcher.commands.keys():
        dispatcher.commands[cmd]()
    else:
        print('Command \'%s\' does not exist.' %(sys.argv[1]))
        print('\tSupported Commands : %s' %(str(list(dispatcher.commands.keys()))))    
    
    