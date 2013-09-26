package cl.telematic.rest;

import cl.telematic.rest.domain.DemandStats;
import cl.telematic.rest.domain.Device;
import cl.telematic.rest.domain.EnergyStats;
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

    public cl.telematic.model.DemandStats toDBDomain(DemandStats rest)
    {
        final cl.telematic.model.DemandStats db = new cl.telematic.model.DemandStats();
        db.setKva_block_demand(rest.getKva_block_demand());
        db.setKvar_import_block_demand(rest.getKvar_import_block_demand());
        db.setKw_import_block_demand(rest.getKw_import_block_demand());
        return db;
    }

    public cl.telematic.model.EnergyStats toDBDomain(EnergyStats rest)
    {
        final cl.telematic.model.EnergyStats db = new cl.telematic.model.EnergyStats();
        db.setKwh_import(rest.getKwh_import());
        db.setKwh_import_l1(rest.getKwh_import_l1());
        db.setKwh_import_l2(rest.getKwh_import_l2());
        db.setKwh_import_l3(rest.getKwh_import_l3());
        return db;
    }

    public List<Device> toRestDeviceList(List<cl.telematic.model.Device> dbs)
    {
        final List<Device> rests = new ArrayList<Device>();
        for (cl.telematic.model.Device db : dbs) {
            rests.add(toRestDomain(db));
        }
        return rests;
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

    private Device toRestDomain(cl.telematic.model.Device db)
    {
        final Device rest = new Device();
        rest.setId(db.getId());
        rest.setIp(db.getIp());
        rest.setName(db.getName());
        return rest;
    }
}
