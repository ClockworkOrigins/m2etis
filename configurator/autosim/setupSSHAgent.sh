test "$SSH_AUTH_SOCK" || exec ssh-agent $SHELL -c "ssh-add; exec $SHELL -login"
