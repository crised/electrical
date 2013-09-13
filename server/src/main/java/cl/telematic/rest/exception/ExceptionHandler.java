package cl.telematic.rest.exception;

import cl.telematic.business.EmailAleadyRegisteredException;
import org.jboss.resteasy.logging.Logger;
import org.jboss.resteasy.spi.BadRequestException;

import javax.ejb.EJBAccessException;
import javax.ejb.EJBTransactionRolledbackException;
import javax.persistence.NoResultException;
import javax.validation.ConstraintViolationException;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.SecurityContext;
import javax.ws.rs.ext.ExceptionMapper;
import javax.ws.rs.ext.Provider;
import javax.ws.rs.ext.Providers;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

@Provider
public class ExceptionHandler implements ExceptionMapper<Exception> {

    private static final Logger LOGGER = Logger.getLogger(ExceptionHandler.class);

    @Context
    private Providers providers;

    @Context
    private SecurityContext securityContext;

    @Override
    public Response toResponse(Exception exception)
    {
        if (exception instanceof EJBTransactionRolledbackException) {
            Throwable throwable = exception;
            while (null != throwable.getCause()) {
                throwable = throwable.getCause();
            }
            if (throwable instanceof ConstraintViolationException) {
                final ExceptionMapper<ConstraintViolationException> exceptionMapper = providers.getExceptionMapper(ConstraintViolationException.class);
                if (null != exceptionMapper) {
                    return exceptionMapper.toResponse((ConstraintViolationException) throwable);
                }
            }
        }
        if (exception instanceof EmailAleadyRegisteredException) {
            return buildResponse(Response.Status.PRECONDITION_FAILED.getStatusCode(), "Email already registered");
        } else if (exception instanceof BadRequestException) {
            return handleBadRequestException((BadRequestException) exception);
        } else if (exception instanceof NoResultException) {
            return Response.status(Response.Status.NOT_FOUND).build();
        } else if (exception instanceof WebApplicationException) {
            return ((WebApplicationException) exception).getResponse();
        } else if (exception instanceof EJBAccessException) {
            final Response.Status status = null == securityContext.getUserPrincipal() ? Response.Status.UNAUTHORIZED : Response.Status.FORBIDDEN;
            final Response.ResponseBuilder responseBuilder = Response.status(status);
            return responseBuilder.build();
        }
        LOGGER.error("Handline unexpected exception", exception);
        return Response.status(Response.Status.INTERNAL_SERVER_ERROR).build();
    }

    private Response buildResponse(int statusCode, String message)
    {
        return Response.status(statusCode).entity(message).type(MediaType.TEXT_PLAIN_TYPE).build();
    }

    private Response handleBadRequestException(BadRequestException exception)
    {
        @SuppressWarnings("ThrowableResultOfMethodCallIgnored")
        final String pattern = "Unable to extract parameter from http request[^\"]+(\"\\w+\")";
        final String exceptionMessage = exception.getMessage();
        final Matcher matcher = Pattern.compile(pattern).matcher(exceptionMessage);
        String message;
        if (matcher.find()) {
            message = "Unable to extract parameter " + matcher.group(1);
        } else if (exceptionMessage.startsWith("Failed processing arguments")) {
            message = "Failed processing arguments";
        } else {
            message = "Invalid request";
            LOGGER.info("Handling unusual BadRequestException: " + exceptionMessage);
        }
        if (exceptionMessage.startsWith("Unable to extract parameter")) {
            return buildResponse(exception.getErrorCode(), message);
        } else {
            return buildResponse(exception.getErrorCode(), message);
        }
    }
}
