use std::env;
use once_cell::sync::Lazy;

static THIS: &str = file!();
static ARGS: Lazy<Vec<String>> = Lazy::new(|| env::args().collect());
static THIS2: Lazy<&String> = Lazy::new(|| &ARGS[0]);

fn main() {
    let args: Vec<String> = env::args().collect();

    for (i, a) in args.iter().enumerate() {
        println!("{i}: {a}")
    }
    let this: &str = file!();
    println!("defined in file: [{this}] [{THIS}] [{}]", &*THIS2);
}
