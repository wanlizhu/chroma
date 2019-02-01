import sys
import getopt

class CommandDispatcher(object):
    def __init__(self):
        self.commands = {
            'build' : self.build,
            'demo' : self.demo,
        }

    def build(self, args):
        return

    def demo(self, args):
        return


if __name__=='__main__':
    if len(sys.argv) <= 1:
        args = input('>> ').split(None, -1)
        cmd = args[0]
        args.pop(0)
    else:
        cmd = sys.argv[1]
        args = sys.argv[2:]
    
    dispatcher = CommandDispatcher()
    if cmd in dispatcher.commands.keys():
        dispatcher.commands[cmd](args)
    else:
        print('Command \'%s\' does not exist.' %(cmd))
        print('\tSupported Commands : %s' %(str(list(dispatcher.commands.keys()))))    
    
    