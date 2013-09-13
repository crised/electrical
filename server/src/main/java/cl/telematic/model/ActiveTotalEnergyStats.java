/*
 * JBoss, Home of Professional Open Source
 * Copyright 2013, Red Hat, Inc. and/or its affiliates, and individual
 * contributors by the @authors tag. See the copyright.txt in the
 * distribution for a full listing of individual contributors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package cl.telematic.model;

import javax.persistence.Column;
import javax.persistence.DiscriminatorValue;
import javax.persistence.Entity;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
@Entity
@DiscriminatorValue("ATE")
public class ActiveTotalEnergyStats extends StatsEntry {

    @NotNull
    @Column(name = "ACTIVE_TOTAL_ENERGY")
    private Integer activeTotalEnergy;

    public Integer getActiveTotalEnergy()
    {
        return activeTotalEnergy;
    }

    public void setActiveTotalEnergy(Integer activeTotalEnergy)
    {
        this.activeTotalEnergy = activeTotalEnergy;
    }
}
