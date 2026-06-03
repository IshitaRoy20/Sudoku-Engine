#ifndef SRC_GRID_H_
#define SRC_GRID_H_

#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "coord.h"

namespace sudoku {

class Grid {

private:

    std::array<
        std::array<int,9>,
        9
    > grid{};

    std::set<Coord>
        prefilled_cells;

public:

    Grid()
    {
        for(auto& row : grid)
            row.fill(0);
    }

    explicit Grid(
        const std::array<
            std::array<int,9>,
            9
        >& initial_grid
    )
    {
        set_initial_state(
            initial_grid
        );
    }

    void set_initial_state(
        const std::array<
            std::array<int,9>,
            9
        >& initial_grid
    )
    {
        prefilled_cells.clear();

        for(int row=0;
            row<GRID_LEN;
            row++)
        {
            for(int col=0;
                col<GRID_LEN;
                col++)
            {
                int value =
                    initial_grid
                    [row][col];

                if(value < 0 ||
                   value > 9)
                {
                    throw
                    std::invalid_argument(
                        "Invalid cell value."
                    );
                }

                grid[row][col] =
                    value;

                if(value != 0)
                {
                    prefilled_cells
                    .insert({
                        row,col
                    });
                }
            }
        }
    }

    void set_initial_state_from_file(
        const std::string&
        filename
    )
    {
        std::ifstream file(
            filename
        );

        if(!file)
        {
            throw
            std::invalid_argument(
                "Cannot open file."
            );
        }
        std::array<std::array<int,9>,9> temp{};

        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(!(file >>
                     temp[i][j]))
                {
                    throw
                    std::invalid_argument(
                        "Invalid file."
                    );
                }
            }
        }

        set_initial_state(
            temp
        );
    }

    int get(Coord coord)
    const
    {
        return grid
            [coord.first]
            [coord.second];
    }

    void update(
        Coord coord,
        int value
    )
    {
        if(value < 0 ||
           value > 9)
        {
            throw
            std::invalid_argument(
                "Value must be 0-9."
            );
        }

        grid
        [coord.first]
        [coord.second]
        = value;
    }

    bool coord_was_pre_filled(
        Coord coord
    ) const
    {
        return
        prefilled_cells
        .count(coord);
    }

    bool is_empty(
        Coord coord
    ) const
    {
        return get(coord)==0;
    }

    bool value_exists_in_row(
        Coord coord,
        int value
    ) const
    {
        for(int col=0;
            col<9;
            col++)
        {
            if(col ==
               coord.second)
                continue;

            if(grid
               [coord.first]
               [col]
               == value)
            {
                return true;
            }
        }

        return false;
    }

    bool value_exists_in_column(
        Coord coord,
        int value
    ) const
    {
        for(int row=0;
            row<9;
            row++)
        {
            if(row ==
               coord.first)
                continue;

            if(grid
               [row]
               [coord.second]
               == value)
            {
                return true;
            }
        }

        return false;
    }

    bool value_exists_in_box(
        Coord coord,
        int value
    ) const
    {
        int rs =
            (coord.first/3)*3;

        int cs =
            (coord.second/3)*3;

        for(int r=rs;
            r<rs+3;
            r++)
        {
            for(int c=cs;
                c<cs+3;
                c++)
            {
                if(r==
                   coord.first
                   &&
                   c==
                   coord.second)
                {
                    continue;
                }

                if(grid[r][c]
                   == value)
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool is_valid_placement(
        Coord coord,
        int value
    ) const
    {
        return
        !value_exists_in_row(
            coord,
            value
        )
        &&
        !value_exists_in_column(
            coord,
            value
        )
        &&
        !value_exists_in_box(
            coord,
            value
        );
    }

    std::vector<int>
    get_possible_values(
        Coord coord
    ) const
    {
        std::vector<int>
            result;

        for(int value=1;
            value<=9;
            value++)
        {
            if(
                is_valid_placement(
                    coord,
                    value
                )
            )
            {
                result.push_back(
                    value
                );
            }
        }

        return result;
    }

    bool operator==(
        const Grid& other
    ) const
    {
        return
        grid == other.grid;
    }

    friend std::ostream&
    operator<<(
        std::ostream& out,
        const Grid& grid
    );
};

inline std::ostream&
operator<<(
    std::ostream& out,
    const Grid& grid
)
{
    out
    << "+-------+-------+-------+\n";

    for(int r=0;r<9;r++)
    {
        out << "|";

        for(int c=0;c<9;c++)
        {
            out
            << " "
            << grid.grid[r][c];

            if(c%3==2)
                out<<" |";
        }

        out<<"\n";

        if(r%3==2)
        {
            out
            << "+-------+-------+-------+\n";
        }
    }

    return out;
}
}
#endif