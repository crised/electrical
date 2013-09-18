package cl.telematic.business;

import javax.ejb.ApplicationException;

@ApplicationException(rollback = true)
public class InvalidCredentialsException extends Exception {

    public InvalidCredentialsException(String msg)
    {
        super(msg);
    }
}
