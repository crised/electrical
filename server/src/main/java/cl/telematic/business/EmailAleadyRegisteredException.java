package cl.telematic.business;

import javax.ejb.ApplicationException;

@ApplicationException(rollback = true)
public class EmailAleadyRegisteredException extends Exception {

    public EmailAleadyRegisteredException(String message)
    {
        super(message);
    }
}
