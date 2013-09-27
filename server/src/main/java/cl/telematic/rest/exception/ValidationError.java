package cl.telematic.rest.exception;

import javax.validation.ConstraintViolation;
import javax.validation.ConstraintViolationException;
import javax.validation.Path;
import javax.xml.bind.annotation.XmlElement;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class ValidationError {

    private List<ConstraintViolationWrap> constraintViolations;

    /**
     * Required by JAXB.
     */
    @SuppressWarnings("UnusedDeclaration")
    public ValidationError()
    {
    }

    public ValidationError(ConstraintViolationException exception)
    {
        this.constraintViolations = new ArrayList<ConstraintViolationWrap>();
        for (ConstraintViolation violation : exception.getConstraintViolations()) {
            this.constraintViolations.add(new ConstraintViolationWrap(violation));
        }
    }

    public List<ConstraintViolationWrap> getConstraintViolations()
    {
        return constraintViolations;
    }

    @Override
    public String toString()
    {
        return "ValidationError{" +
            "constraintViolations=" + constraintViolations +
            '}';
    }

    public static class ConstraintViolationWrap {

        @XmlElement
        private Object invalidValue;

        @XmlElement
        private String message;

        @XmlElement
        private String propertyPath;

        /**
         * This constructor is required by JSON parser to deserialize message.
         */
        @SuppressWarnings("UnusedDeclaration")
        public ConstraintViolationWrap()
        {
        }

        public ConstraintViolationWrap(ConstraintViolation exception)
        {
            this.invalidValue = exception.getInvalidValue();
            this.message = exception.getMessage();
            final Iterator<Path.Node> iterator = exception.getPropertyPath().iterator();
            final StringBuilder path = new StringBuilder();
            while (iterator.hasNext()) {
                path.append(iterator.next().getName()).append(".");
            }
            this.propertyPath = path.length() > 0 ? path.substring(0, path.length() - 1) : path.toString();
        }

        public Object getInvalidValue()
        {
            return invalidValue;
        }

        public String getMessage()
        {
            return message;
        }

        public String getPropertyPath()
        {
            return propertyPath;
        }

        @Override
        public String toString()
        {
            return "ConstraintViolationWrap{" +
                "invalidValue=" + invalidValue +
                ", message='" + message + '\'' +
                ", propertyPath='" + propertyPath + '\'' +
                '}';
        }
    }
}
