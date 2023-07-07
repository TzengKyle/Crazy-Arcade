#ifndef SETTINGS_H
#define SETTINGS_H


namespace sizes {
    enum gameStats {
        Rows = 11,
        Columns = 11,
        FieldSize = 50,
        Players = 2,
        GameTime = 180,
        Frequency = 3,
        Strat_pixel_x = 144,
        Strat_pixel_y = 72
    };
    enum playerStats {
        IniBombsNum = 1,
        MaximumBombsNum = 10,
        IniExplosionRange = 1,
        MaximumExplosionRange = 10,
        Health = 3
    };
    enum propStats{
        no_prop = 70,
        Increaserange_prop = no_prop + 10,
        Increasemaxbombs_prop = Increaserange_prop + 10,
        Superstar_prop = Increasemaxbombs_prop + 5,
        Dizzy_prop = Superstar_prop + 5,

        Superstar_time = 3,
        Dizzy_time = 3
    };
}


#endif // SETTINGS_H
