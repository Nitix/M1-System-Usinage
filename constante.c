const char * getError() {
    switch(errno){
    	case EACCES:
			return "The calling process does not have write permission on the message queue, and does not have the CAP_IPC_OWNER capability.";
		case EAGAIN:
			return "The message can't be sent due to the msg_qbytes limit for the queue and IPC_NOWAIT was specified in msgflg.";
		case EFAULT:
			return "The address pointed to by msgp isn't accessible.";
		case EIDRM:
			return "The message queue was removed.";
		case EINTR:
			return "Sleeping on a full message queue condition, the process caught a signal.";
		case EINVAL:
			return "Invalid msqid value, or non-positive mtype value, or invalid msgsz value (less than 0 or greater than the system value MSGMAX).";
		case ENOMEM:
			return "The system does not have enough memory to make a copy of the message pointed to by msgp.";
		case ENOMSG:
			return "IPC_NOWAIT was specified in msgflg and no message of the requested type existed on the message queue.";
    }
    return "";
}