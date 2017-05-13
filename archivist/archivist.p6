use experimental :pack;

sub USAGE() {
    print q:c:to/EOF/;
        Usage:
          {$*PROGRAM-NAME} <folder> [--host=HOST --port=PORT]

        Options:
          --host    Specify the hostname to listen onto.
          --port    Specify the port to listen to.

        Arguments:
          <folder>  Path to a folder used as the database storage. The <folder>
                    is created if it does not exist.
        EOF
}

sub MAIN($folder, :$host = 'localhost', :$port = 4242) {
    my $stg = $folder.IO;
    unless $stg.d {
        note "Creating folder `$stg'...";
        $stg.mkdir
    }
    note "Using folder `$stg' as storage.";
    note "Creating socket on $host:$port..."
    react {
        whenever IO::Socket::Async.listen($host, $port) -> $sock {
            whenever $sock.Supply(:bin) -> $buf {
                my $nbt = Nbt.new($buf)
            }
        }
    }
}

class Nbt {
    has $.data;

    method new(Buf $buf) {
        
    }
}
