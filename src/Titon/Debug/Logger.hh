<?hh // strict
/**
 * @copyright   2010-2015, The Titon Project
 * @license     http://opensource.org/licenses/bsd-license.php
 * @link        http://titon.io
 */

namespace Titon\Debug;

use Psr\Log\AbstractLogger;
use Psr\Log\LogLevel;
use Titon\Debug\Exception\InvalidDirectoryException;
use Titon\Debug\Exception\UnwritableDirectoryException;
use Titon\Utility\Path;
use Titon\Utility\State\Server;
use Titon\Utility\Str;
use \DateTime;
use \Exception;

/**
 * Default logger that inherits functionality from the PSR-3 Logger interfaces.
 * Will log messages to the local file system.
 *
 * @package Titon\Debug
 */
class Logger extends AbstractLogger {

    const string EMERGENCY = LogLevel::EMERGENCY; // 0
    const string ALERT = LogLevel::ALERT; // 1
    const string CRITICAL = LogLevel::CRITICAL; // 2
    const string ERROR = LogLevel::ERROR; // 3
    const string WARNING = LogLevel::WARNING; // 4
    const string NOTICE = LogLevel::NOTICE; // 5
    const string INFO = LogLevel::INFO; // 6
    const string DEBUG = LogLevel::DEBUG; // 7

    /**
     * Directory to store log files.
     *
     * @var string
     */
    protected string $directory;

    /**
     * Set the directory to log to.
     *
     * @uses Titon\Utility\Path
     *
     * @param string $dir
     * @throws \Titon\Debug\Exception\InvalidDirectoryException
     * @throws \Titon\Debug\Exception\UnwritableDirectoryException
     */
    public function __construct(string $dir) {
        if (!is_dir($dir)) {
            throw new InvalidDirectoryException(sprintf('Invalid log directory %s', $dir));

        } else if (!is_writable($dir)) {
            throw new UnwritableDirectoryException('Log directory is not writable');
        }

        $this->directory = Path::ds($dir, true);
    }

    /**
     * Return the directory.
     *
     * @return string
     */
    public function getDirectory(): string {
        return $this->directory;
    }

    /**
     * Log a message by default to the file system. The message will be sent to the directory
     * passed through the constructor into a file that matches the level name.
     * Should conform to PSR-3 and RFC 5424.
     *
     * @link https://github.com/php-fig/fig-standards/blob/master/accepted/PSR-3-logger-interface.md
     * @link http://tools.ietf.org/html/rfc5424
     *
     * @param mixed $level
     * @param string $message
     * @param array $context
     * @return void
     */
    public function log(/* HH_FIXME[4032]: no type hint */ $level, /* HH_FIXME[4032]: no type hint */ $message, array<string, mixed> $context = []): void {
        file_put_contents(
            sprintf('%s%s-%s.log', $this->getDirectory(), $level, date('Y-m-d')),
            static::createMessage($level, $message, $context),
            FILE_APPEND);
    }

    /**
     * Sharable message parsing and building method. Conforms to the PSR spec.
     *
     * @uses Titon\Utility\Str
     *
     * @param mixed $level
     * @param string $message
     * @param array $context
     * @return string
     */
    public static function createMessage(mixed $level, string $message, array<string, mixed> $context = []): string {
        $exception = null;
        $url = '';

        if (array_key_exists('exception', $context)) {
            $exception = $context['exception'];
            unset($context['exception']);
        }

        if (array_key_exists('url', $context)) {
            $url = $context['url'];

        } else if ($pathInfo = Server::get('PATH_INFO')) {
            $url = $pathInfo;

        } else if ($reqUri = Server::get('REQUEST_URI')) {
            $url = $reqUri;
        }

        $message = sprintf('[%s] %s %s',
            date(DateTime::RFC3339),
            Str::insert($message, new Map($context), Map {'escape' => false}),
            $url ? '[' . (string) $url . ']' : '') . PHP_EOL;

        if ($exception instanceof Exception) {
            $message .= $exception->getTraceAsString() . PHP_EOL;
        }

        return $message;
    }

}
