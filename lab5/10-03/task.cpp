#include <boost/multi_array.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using Grid = boost::multi_array<bool, 2>;

class GameOfLife
{
public:
    GameOfLife(std::size_t rows, std::size_t cols)
        : m_grid(boost::extents[rows][cols])
        , m_rows(rows)
        , m_cols(cols)
    {
    }

    void set_cell(std::size_t row, std::size_t col, bool state)
    {
        if (row < m_rows && col < m_cols)
            m_grid[row][col] = state;
    }

    void setup_glider()
    {
        set_cell(1, 2, true);
        set_cell(2, 3, true);
        set_cell(3, 1, true);
        set_cell(3, 2, true);
        set_cell(3, 3, true);
    }

    void setup_blinker()
    {
        set_cell(4, 3, true);
        set_cell(4, 4, true);
        set_cell(4, 5, true);
    }

    void setup_gosper_glider_gun()
    {
        set_cell(5, 1, true);  set_cell(5, 2, true);
        set_cell(6, 1, true);  set_cell(6, 2, true);

        set_cell(3, 13, true); set_cell(3, 14, true);
        set_cell(4, 12, true); set_cell(4, 16, true);
        set_cell(5, 11, true); set_cell(5, 17, true);
        set_cell(6, 11, true); set_cell(6, 17, true);
        set_cell(7, 11, true); set_cell(7, 17, true);
        set_cell(8, 12, true); set_cell(8, 16, true);
        set_cell(9, 13, true); set_cell(9, 14, true);

        set_cell(1, 25, true);
        set_cell(2, 23, true); set_cell(2, 25, true);
        set_cell(3, 21, true); set_cell(3, 22, true);
        set_cell(4, 21, true); set_cell(4, 22, true);
        set_cell(5, 21, true); set_cell(5, 22, true);
        set_cell(6, 23, true); set_cell(6, 25, true);
        set_cell(7, 25, true);

        set_cell(3, 35, true); set_cell(3, 36, true);
        set_cell(4, 35, true); set_cell(4, 36, true);
    }

    auto count_neighbors(std::size_t row, std::size_t col) const -> std::size_t
    {
        std::size_t count = 0;
        for (int dr = -1; dr <= 1; ++dr)
        {
            for (int dc = -1; dc <= 1; ++dc)
            {
                if (dr == 0 && dc == 0) continue;
                int nr = static_cast<int>(row) + dr;
                int nc = static_cast<int>(col) + dc;
                if (nr >= 0 && nr < static_cast<int>(m_rows) &&
                    nc >= 0 && nc < static_cast<int>(m_cols))
                {
                    count += m_grid[nr][nc] ? 1 : 0;
                }
            }
        }
        return count;
    }

    void step()
    {
        Grid next(boost::extents[m_rows][m_cols]);
        for (std::size_t i = 0; i < m_rows; ++i)
        {
            for (std::size_t j = 0; j < m_cols; ++j)
            {
                auto neighbors = count_neighbors(i, j);
                if (m_grid[i][j])
                    next[i][j] = (neighbors == 2 || neighbors == 3);
                else
                    next[i][j] = (neighbors == 3);
            }
        }
        m_grid = std::move(next);
    }

    void print(std::ostream& os = std::cout) const
    {
        os << std::string(m_cols * 2 + 1, '-') << '\n';
        for (std::size_t i = 0; i < m_rows; ++i)
        {
            os << '|';
            for (std::size_t j = 0; j < m_cols; ++j)
            {
                os << (m_grid[i][j] ? "O " : "  ");
            }
            os << "|\n";
        }
        os << std::string(m_cols * 2 + 1, '-') << '\n';
    }

    auto rows() const { return m_rows; }
    auto cols() const { return m_cols; }

private:
    Grid m_grid;
    std::size_t m_rows;
    std::size_t m_cols;
};

int main()
{
    constexpr std::size_t rows = 10;
    constexpr std::size_t cols = 10;
    constexpr std::size_t generations = 20;

    GameOfLife game(rows, cols);
    game.setup_glider();

    std::cout << "Conway's Game of Life - " << rows << "x" << cols << " grid\n";
    std::cout << "Pattern: Glider\n\n";

    for (std::size_t gen = 0; gen < generations; ++gen)
    {
        std::cout << "Generation " << gen << ":\n";
        game.print();
        std::cout << '\n';
        game.step();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}