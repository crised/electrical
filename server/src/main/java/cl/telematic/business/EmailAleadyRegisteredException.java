package cl.telematic.business;

import javax.ejb.ApplicationException;

@ApplicationException(rollback = true)
public class EmailAleadyRegisteredException extends RuntimeException {

    public EmailAleadyRegisteredException(String message)
    {
        super(message);
    }
}
