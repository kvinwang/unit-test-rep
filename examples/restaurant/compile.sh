gcc src/chef.c src/waiter.c tests/test.c -lcmocka -Isrc -Wl,-wrap,chef_cook

