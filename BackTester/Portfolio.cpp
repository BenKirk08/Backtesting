#include "Portfolio.h"
#include "Strategy.h"

Portfolio runBackTest(const std::vector<Bar>& bars, const std::vector<Signal>& signals, double startingMoney) {
    Portfolio portfolio;
    portfolio.cash = startingMoney; //start with given cash
    portfolio.shares = 0; //start w/no shares

    Trade currentTrade; //tracks current open trade
    int signalIndex = 0; //tarck whihc signal on

    for (int i = 0; i < bars.size(); i++) {
        //check if signal on this bar
        if (signalIndex < signals.size() && signals[signalIndex].barIndex == i) {
            const Signal& signal = signals[signalIndex];
            signalIndex++;

            if (signal.buy && portfolio.shares == 0) {
                //BUY -> use all cash to buy shares
                portfolio.shares = portfolio.cash / signal.price; //Max amount shares can buy
                portfolio.cash = 0;
                currentTrade.buyDate = bars[i].date;
                currentTrade.buyPrice = signal.price;
            }
            else if (!signal.buy && portfolio.shares > 0) {
                //Sell -> convert all shares to cash
                portfolio.cash = portfolio.shares * signal.price;
                currentTrade.sellDate = bars[i].date;
                currentTrade.sellPrice = signal.price;
                currentTrade.profitLoss = (currentTrade.sellPrice - currentTrade.buyPrice) * portfolio.shares;
                portfolio.shares = 0;
                portfolio.trades.push_back(currentTrade);
            }
        }
        //track portfolio value every day
        double totalValue = portfolio.cash + (portfolio.shares * bars[i].close);
        portfolio.valueOverTime.push_back({bars[i].date, totalValue});
    }
    return portfolio;
}


