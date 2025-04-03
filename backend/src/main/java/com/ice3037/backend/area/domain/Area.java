package com.ice3037.backend.area.domain;

import jakarta.persistence.*;
import lombok.*;
import lombok.extern.slf4j.Slf4j;

@Entity
@Slf4j
@Getter
@Builder(toBuilder = true)
@AllArgsConstructor(access = AccessLevel.PRIVATE)
@NoArgsConstructor(access = AccessLevel.PROTECTED)
public class Area {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    @Column
    private Double latitude1;

    @Column
    private Double longitude1;

    @Column
    private Double latitude2;

    @Column
    private Double longitude2;

    @Column
    private Double latitude3;

    @Column
    private Double longitude3;

    @Column
    private Double latitude4;

    @Column
    private Double longitude4;

    @Column
    private boolean isActive;

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Area area = (Area) o;
        return id.equals(area.id);
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }
}
