#include <chrono>
#include <helpers.hpp>
#include <pthread.h>


pthread_mutex_t m;


void* run_job(void *args)
{	
	std::string job_definition {static_cast<const char*>(args)};
	//std::cout<<job_definition <<std::endl;
	Job user_job {convert_job_string_to_vector(job_definition)};
	const auto tmp_date_obj = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::vector<std::string> actual_date = convert_job_string_to_vector(std::ctime(&tmp_date_obj));
	
	pthread_mutex_lock(&m);
	if ( user_job.is_day_of_week(actual_date.at(0)) && user_job.is_month(actual_date.at(1))
		&& user_job.is_month_day(actual_date.at(2)) && user_job.is_time(actual_date.at(3)) )
	{
		if (user_job.month_day != "29" && user_job.month != "2")
		{
			user_job.call_job();
			pthread_mutex_unlock(&m);
		}
		else
		{
			user_job.call_job();
			std::cout<<"What a funny day!";
		}
	}
	else
	{
		pthread_mutex_unlock(&m);
	}
	//std::cout<<std::ctime(&tmp_date_obj) <<std::endl;
	//std::cout<< user_job <<std::endl;
	return nullptr;
}

int main(int argc, char* argv[])
{
	pthread_t t1, t2;
	char job_definition[] {"59 23 29 2 Sut echo 'Job is done'"};
	char job_definition2[] {"* * * * * echo 'Job2 is done'"};

	const unsigned max = 10;
	for (unsigned i = max; i > 0; --i)
	{
		std::cout <<"Iteration " <<max - i <<std::endl;
		pthread_create(&t1, NULL, run_job, static_cast<void*>(job_definition));
		pthread_create(&t2, NULL, run_job, static_cast<void*>(job_definition2));
		pthread_join(t1, NULL);
		pthread_join(t2, NULL);
	}
	return 0;
}