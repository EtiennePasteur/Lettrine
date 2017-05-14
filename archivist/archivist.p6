my constant VER = v0.1.0;

class Nbt {
    use experimental :pack;
    has @.data = ();

    enum Tag (
        String => 8
    );

    submethod TWEAK(IO::Socket:D :$sock) {
        while my $tag = $sock.read(1).head {
            given $tag {
                when (Tag::String) {
                    my $len = .unpack: 'n' with $sock.read: 2 || last;
                    my $str = .decode with $sock.read: $len || last;
                    @!data.push: $str
                }
                default { die 'Invalid NBT tag: ' ~ $_ }
            }
        }
    }
}

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

        Lettrine v{VER} - Open Library Hackathon 2017
        EOF
}

sub MAIN($folder, :$host = '127.0.0.1', :$port = 4242) {
    my $stg = $folder.IO;
    unless $stg.d {
        note "Creating folder `$stg'...";
        $stg.mkdir
    }
    note "Using folder `$stg' as storage.";

    note "Creating socket on $host:$port...";
    my $listener = IO::Socket::INET.new(
        :listen,
        :localhost($host),
        :localport($port)
    );
    END $listener.close;
    note 'Created socket.';

    loop {
        my $sock = $listener.accept;
        try {
            my $nbt := Nbt.new :$sock;
            dd $nbt;
            CATCH { default {
                $sock.print: 'ko';
                warn "WARNING: {.message}\nas {.^name}\n{.backtrace}"
            }}
        }
        $sock.close;
    }
}
