package cl.telematic.business;

import javax.ejb.ApplicationException;

@ApplicationException(rollback = true)
public class InvalidCredentialsException extends RuntimeException {

    public InvalidCredentialsException(String msg)
    {
        super(msg);
    }
}
