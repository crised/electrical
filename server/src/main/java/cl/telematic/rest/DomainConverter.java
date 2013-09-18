package cl.telematic.rest;

import cl.telematic.rest.domain.User;

import javax.annotation.Nonnull;
import javax.ejb.Stateless;
import java.util.ArrayList;
import java.util.List;

@Stateless
public class DomainConverter {

    @Nonnull
    public cl.telematic.model.User toDBDomain(@Nonnull User rest)
    {
        final cl.telematic.model.User db = new cl.telematic.model.User();
        db.setId(rest.getId());
        db.setName(rest.getName());
        db.setPhoneNumber(rest.getPhoneNumber());
        db.setEmail(rest.getEmail());
        return db;
    }

    @Nonnull
    public User toRestDomain(@Nonnull cl.telematic.model.User db)
    {
        final User rest = new User();
        rest.setId(db.getId());
        rest.setEmail(db.getEmail());
        rest.setName(db.getName());
        rest.setPhoneNumber(db.getPhoneNumber());
        return rest;
    }

    @Nonnull
    public List<User> toUserList(@Nonnull List<cl.telematic.model.User> dbs)
    {
        final List<User> rests = new ArrayList<User>();
        for (cl.telematic.model.User db : dbs) {
            rests.add(toRestDomain(db));
        }
        return rests;
    }
}
