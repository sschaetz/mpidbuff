#include <vector>
#include <algorithm>
#include <cstring>
#include <numeric>

#include <boost/timer.hpp>

#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/iterator_range.hpp>

#include <lib/window.hpp>
#include <lib/masterworker/masterworker.hpp>

#include <dev/async_remote_range.hpp>

#define PACKAGES 512

using namespace masterworker;
namespace mpi = boost::mpi;
using namespace boost::fusion;
using namespace boost::lambda;


WORKER_FUNCTION(float, workerfunc, (std::size_t, size))
{

  float result = 0.0;
  std::vector<float> data(size, 0.0);
  mpi::window win(&*data.begin(), 1024, comm);
  int iterations = size / 1024 / (comm.size()-1);
  int offset = iterations * 1024 * (comm.rank()-1);

  for(int i=0; i<iterations; i++)
  {
    win.lock(mpi::lock_shared, 0);
    win.get(&*data.begin(), 1024, 0, offset + i * 1024);
    win.unlock(0);
    result += std::accumulate(data.begin(), data.end(), 0);
  }

  return result;
}

template <typename T>
void printvector(T i)
{
  std::cout << " " << i;
}

void masterloop(boost::mpi::communicator & comm)
{
  std::vector<float> data(1024*PACKAGES, 1.0);

  async_remote_range<float>
    r(PACKAGES, 1024, (PACKAGES/4 * (_1-1) + _2), comm);

  r.init(boost::iterator_range<std::vector<float>::iterator>
         (data.begin(), data.end()));

  boost::timer time;
  double time_elapsed = 0.0;

  for(int i=0; i<9; i++)
  {
    run_async<worker::workerfunc>(make_vector(1024*PACKAGES), comm);

    {
      mpi::window win((float*)&data[0], 1024*PACKAGES, comm);
    }

    std::vector<worker::workerfunc::return_type> v =
      sync<worker::workerfunc>(comm);
  }

  run_async<worker::workerfunc>(make_vector(1024*PACKAGES), comm);

  {
    mpi::window win((float*)&data[0], 1024*PACKAGES, comm);
  }

  std::vector<worker::workerfunc::return_type> v =
    sync<worker::workerfunc>(comm);

  time_elapsed = time.elapsed();
  std::cout << "Accumulated result: " <<
    std::accumulate(v.begin(), v.end(), 0) << " took " <<
    std::setprecision(9) << time_elapsed << std::endl;
  quit(comm);

  time_elapsed = time.elapsed();
  worker::workerfunc::return_type v2;
  for(int i=0; i<10; i++)
  {
    v2 += std::accumulate(data.begin(), data.end(), 0);
  }
  time_elapsed = time.elapsed() - time_elapsed;
  std::cout << "Accumulated result: " << v2/10 << " took " <<
    std::setprecision(9) << time_elapsed << std::endl;
}

int main(int argc, char* argv[])
{

  mpi::environment env(argc, argv);
  mpi::communicator world;

  if(world.size() < 3 || (world.size() % 2) != 1)
  {
    std::cout << "Launch this test with a number of node that "
      "divides PACKAGES without rest +1 (for master node)" << std::endl;
    exit(1);
  }

  if(world.rank() == 0)
  {
    masterloop(world);
  }
  else if(world.rank() != 0)
  {
    workerloop(world, 0);
  }

}


