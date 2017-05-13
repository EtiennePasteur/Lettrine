my constant VER = v0.1.0;

class Nbt {
    use experimental :pack;
    has @.data = ();

    enum Tag (
        String => 8
    );

    submethod TWEAK(Buf :$buf) {
        my $hd = $buf.head;
        my $tl = $buf.subbuf(1);
        while ?$tl {
            my $to-skip = do given $hd {
                when Tag::String {
                    my $len = $tl.unpack: 'n';
                    my $str = $tl.unpack: "x2 a$len";
                    @!data.push: $str;
                    1 + 2 + $len
                }
                default { die 'Invalid NBT tag: ' ~ $_ }
            }
            $hd = $tl[$to-skip - 1];
            $tl .= subbuf($to-skip)
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
    my $lock = Lock.new;
    state $main-id;
    react {
        whenever IO::Socket::Async.listen($host, $port) -> $conn {
            my $id = $lock.protect({ $main-id++ }) ~ '#' ~ $*THREAD.id;
            note "Client $id connected!";
            whenever $conn.Supply(:bin) -> $buf {
                CATCH { default {
                    warn "ERROR IN REQUEST: {.Str}\n{.backtrace}";
                    await $conn.write: Blob.new: 'ko'.ords;
                }}
                QUIT { default { note "Client $id disconnected." } }
                my $nbt = Nbt.new :$buf;
            }
        }
    }
}
